/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include "Tetris/Systems/GameLogicSystem/GameLogicSystem.h"

#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>
#include <cmath>

#include "Elg/Utilities/StringHash/Hash.h"
#include "Elg/EntityManager/EntityManager.h"

#include "Tetris/Components/AllComponents.h"
#include "Tetris/Assets/AllAssets.h"
#include "Tetris/Helpers/TetrisHelper.h"
#include "Tetris/Systems/GameLogicSystem/GameLogicSystemProfiling.h"

GameLogicSystem::GameLogicSystem(TetrisEntityManager *entity_manager)
    : m_entity_manager_(entity_manager), m_accumulated_time_(0), m_fall_period_(Elg::Clock::Milliseconds(500)),
      m_level_(1), m_lines_(0), m_next_tetromino_() {
}

GameLogicSystem::~GameLogicSystem() {
}

void GameLogicSystem::Initialize() {
  // Initialize random number generator
  std::srand(static_cast<unsigned int>(std::time(NULL)));

  // Create board
  uint32 board = m_entity_manager_->CreateEntity();

  uint32 board_graphics_offset = m_entity_manager_->CreateComponent<Graphics>(board);
  Graphics* board_graphics = m_entity_manager_->GetData<Graphics>() + board_graphics_offset;
  board_graphics->Representation = BOARD;
  board_graphics->ScreenX = Constants::BOARD_POSITION_X;
  board_graphics->ScreenY = Constants::BOARD_POSITION_Y;
  board_graphics->RepresentationAssetIdHash = ELG_STRING_HASH("BOARD");

  // Create preview
  uint32 preview = m_entity_manager_->CreateEntity();

  uint32 preview_graphics_offset = m_entity_manager_->CreateComponent<Graphics>(preview);
  Graphics* preview_graphics = m_entity_manager_->GetData<Graphics>() + preview_graphics_offset;
  preview_graphics->Representation = PREVIEW;
  preview_graphics->ScreenX = Constants::PREVIEW_POSITION_X;
  preview_graphics->ScreenY = Constants::PREVIEW_POSITION_Y;
  preview_graphics->RepresentationAssetIdHash = ELG_STRING_HASH("PREVIEW");

  // Current piece
  GenerateNextTetromino();
  PlaceNextTetrominoOnBoard();

  // Next piece
  GenerateNextTetromino();

  // Update graphics data on everything
  UpdateGraphicsData();
}

bool GameLogicSystem::Update(Elg::Clock::GameClock::TimePoint /* time */, Elg::Clock::GameClock::Duration timeStep) {
  // Schematic overview:
  //   previousState = currentState;
  //   integrate( currentState, m_current_simulation_time_, DT );
  TETRIS_PROFILE_GAMELOGICSYSTEM_UPDATE;

  // Process movement events
  bool should_continue_after_events = ProcessEvents();
  if (!should_continue_after_events) {
    return false;
  }

  // Process falling down
  m_accumulated_time_ += Elg::Clock::DurationCast<Elg::Clock::Milliseconds>(timeStep);
  if (m_accumulated_time_ >= m_fall_period_) {
    // Perform an many fall steps as needed
    while (m_accumulated_time_ >= m_fall_period_) {
      bool should_continue_after_fall = PerformTetrominoFall();
      if (!should_continue_after_fall) {
        return false;
      }
      m_accumulated_time_ -= m_fall_period_;
    }
  }

  // Update graphics
  UpdateGraphicsData();

  return true;
}

bool GameLogicSystem::ProcessEvents() {
  // Movable pieces have MovementComponent
  TetrisEntitySet movable_entities(m_entity_manager_, m_entity_manager_->GetDataEntities<Movement>(),
                                   m_entity_manager_->GetDataCount<Movement>());

  // Process all moveables
  bool should_continue = true;
  for (uint32 entity_index = 0;entity_index < movable_entities.GetCount();++entity_index) {
    uint32 e = movable_entities[entity_index];
    uint32 m_offset = m_entity_manager_->GetComponent<Movement>(e);
    Movement *m = m_entity_manager_->GetData<Movement>() + m_offset;

    // Process all events for this moveable
    uint32 event_count = m->EventCount;
    uint32 event_index = 0;
    bool process_next_event = true;
    while (event_index < event_count && process_next_event) {
      TetrisEvent current_event = m->Events[event_index];
      if (current_event == MOVE_LEFT) {
        ProcessMovementEvent(-1, 0, e);
      } else if (current_event == MOVE_RIGHT) {
        ProcessMovementEvent(1, 0, e);
      } else if (current_event == ROTATE) {
        ProcessMovementEvent(0, 1, e);
      } else if (current_event == DROP) {
        bool drop_result = ProcessDropEvent(e);
        should_continue = should_continue && drop_result;
        process_next_event = false;
      }
      ++event_index;
    }
    m->EventCount = 0;
  }

  return should_continue;
}

void GameLogicSystem::ProcessMovementEvent(int16 dc, int16 dr, uint32 movable_entity) {
  // Get all tiles
  std::vector<BoardLocation*> tile_locations;
  GetTileLocations(&tile_locations);

  // Get the postion and tetromino components of this movable
  uint32 bp_offset = m_entity_manager_->GetComponent<BoardPosition>(movable_entity);
  uint32 t_offset = m_entity_manager_->GetComponent<Tetromino>(movable_entity);

  if (bp_offset != TetrisEntityManager::InvalidOffset && t_offset != TetrisEntityManager::InvalidOffset) {
    // Get the postion and tetromino components of this movable
    BoardPosition *bp = m_entity_manager_->GetData<BoardPosition>() + bp_offset;
    Tetromino *t = m_entity_manager_->GetData<Tetromino>() + t_offset;

    // Update the position
    BoardLocation transformed_location = bp->Location + BoardOffset(0, dc);
    uint16 transformed_rotation = (bp->Rotation + dr) % 4;

    // Get the offsets for the possibly new roatation
    BoardLocation tetromion_pieces_positions[4];
    TetrisHelper::GetBoardLocations(transformed_location, transformed_rotation, t->Type, tetromion_pieces_positions);

    // Test the positions and update the tetromino position if they are valid
    if (TetrisHelper::AreWithinBoard(tetromion_pieces_positions) &&
        TetrisHelper::AreNotColiding(tetromion_pieces_positions, tile_locations)) {
      bp->Location = transformed_location;
      bp->Rotation = transformed_rotation;
    }
  }
}

bool GameLogicSystem::ProcessDropEvent(uint32 movable_entity) {
  // Get all tiles
  std::vector<BoardLocation*> tile_locations;
  GetTileLocations(&tile_locations);

  // Get the postion and tetromino components of this movable
  uint32 bp_offset = m_entity_manager_->GetComponent<BoardPosition>(movable_entity);
  uint32 t_offset = m_entity_manager_->GetComponent<Tetromino>(movable_entity);

  if (bp_offset != TetrisEntityManager::InvalidOffset && t_offset != TetrisEntityManager::InvalidOffset) {
    // Get the postion and tetromino components of this movable
    BoardPosition* bp = m_entity_manager_->GetData<BoardPosition>() + bp_offset;
    Tetromino* t = m_entity_manager_->GetData<Tetromino>() + t_offset;

    // Extract data from components
    BoardLocation current_location = bp->Location;
    uint16 current_roataion = bp->Rotation;
    TetrominoType type = t->Type;

    // Transform the position via offsets to get the individual tiles positions
    BoardLocation tetromion_pieces_positions[4];
    do {
      // Update the current location
      current_location += BoardOffset(1, 0);

      // Reacalculate the positions of tetromino pieces
      TetrisHelper::GetBoardLocations(current_location, current_roataion, type, tetromion_pieces_positions);
    } while (TetrisHelper::AreWithinBoard(tetromion_pieces_positions) &&
             TetrisHelper::AreNotColiding(tetromion_pieces_positions, tile_locations));

    // We don't have to update rotation
    bp->Location = current_location - BoardOffset(1, 0);

    // After we dropped we need to reset the fall timer...
    m_accumulated_time_ = Elg::Clock::Milliseconds(0);

    // ... and place the tetromino on board
    bool should_continue = PlaceTetrominoOnBoard(movable_entity);
    return should_continue;
  } else {
    // We got a weird entity - fail silently
    return true;
  }
}

bool GameLogicSystem::PerformTetrominoFall() {
  // Fallabel pieces have TetrominoComponent + BoardPositionComponent
  TetrisEntitySet fallable_entities(m_entity_manager_, m_entity_manager_->GetDataEntities<Tetromino>(),
                                    m_entity_manager_->GetDataCount<Tetromino>());
  fallable_entities.Filter<BoardPosition>();

  // Work on each movable entity
  bool should_continue = true;
  for (uint32 fallable_index = 0;fallable_index < fallable_entities.GetCount();++fallable_index) {
    // Extract the entity
    uint32 current_tetromino = fallable_entities[fallable_index];

    // Get the interesting components
    uint32 bp_offset = m_entity_manager_->GetComponent<BoardPosition>(current_tetromino);
    uint32 t_offset = m_entity_manager_->GetComponent<Tetromino>(current_tetromino);

    BoardPosition* bp = m_entity_manager_->GetData<BoardPosition>() + bp_offset;
    Tetromino* t = m_entity_manager_->GetData<Tetromino>() + t_offset;

    // Compute the locations after the drop
    BoardLocation new_locations[4];
    BoardLocation new_base_location = bp->Location + BoardOffset(1, 0);
    TetrisHelper::GetBoardLocations(new_base_location, bp->Rotation, t->Type, new_locations);

    // Get all tiles
    std::vector<BoardLocation*> tile_locations;

    // Get tile positions for testing
    GetTileLocations(&tile_locations);

    // Check for colision with existing tiles
    if (TetrisHelper::AreWithinBoard(new_locations) && TetrisHelper::AreNotColiding(new_locations, tile_locations)) {
      bp->Location = new_base_location;
    } else {
      // Place tetromino on board
      bool placement_ok = PlaceTetrominoOnBoard(current_tetromino);
      should_continue = should_continue && placement_ok;
    }
  }

  return should_continue;
}

void GameLogicSystem::UpdateGraphicsData() {
  // Just do the update
  uint32 graphics_count = m_entity_manager_->GetDataCount<Graphics>();
  const uint32* graphics_entities = m_entity_manager_->GetDataEntities<Graphics>();

  for (uint32 graphics_index = 0;graphics_index < graphics_count;++graphics_index) {
    uint32 current_entity = graphics_entities[graphics_index];
    Graphics* g = m_entity_manager_->GetData<Graphics>() + graphics_index;

    uint32 tile_offset = m_entity_manager_->GetComponent<Tile>(current_entity);
    uint32 t_offset = m_entity_manager_->GetComponent<Tetromino>(current_entity);
    uint32 bp_offset = m_entity_manager_->GetComponent<BoardPosition>(current_entity);

    if (tile_offset != TetrisEntityManager::InvalidOffset) {
      // Tiles
      BoardPosition *bp = m_entity_manager_->GetData<BoardPosition>() + bp_offset;

      g->Representation = TILE;
      g->ScreenX = Constants::BOARD_POSITION_X + Constants::BOARD_BORDER +
                   bp->Location.column * Constants::TILE_WIDTH;
      g->ScreenY = Constants::BOARD_POSITION_Y + Constants::BOARD_BORDER +
                   bp->Location.row * Constants::TILE_HEIGHT;
    } else if ((t_offset != TetrisEntityManager::InvalidOffset) && (bp_offset != TetrisEntityManager::InvalidOffset)) {
      // Tetrominos
      Tetromino *t = m_entity_manager_->GetData<Tetromino>() + t_offset;
      BoardPosition *bp = m_entity_manager_->GetData<BoardPosition>() + bp_offset;

      g->Representation = Constants::TETROMINO_ROTATION_CONVERSION[t->Type][bp->Rotation];
      g->ScreenX = Constants::BOARD_POSITION_X + Constants::BOARD_BORDER +
                   Constants::TETROMINO_RENDERING_OFFSET_COLUMN * Constants::TILE_WIDTH +
                   bp->Location.column * Constants::TILE_WIDTH;
      g->ScreenY = Constants::BOARD_POSITION_Y + Constants::BOARD_BORDER +
                   Constants::TETROMINO_RENDERING_OFFSET_ROW * Constants::TILE_HEIGHT +
                   bp->Location.row * Constants::TILE_HEIGHT;
    } else if ((t_offset != TetrisEntityManager::InvalidOffset) && (bp_offset == TetrisEntityManager::InvalidOffset)) {
      // Preview pieces
      Tetromino *t = m_entity_manager_->GetData<Tetromino>() + t_offset;

      uint16 rotation = Constants::TETROMINO_PREVIEW_ROTATION;
      g->Representation = Constants::TETROMINO_ROTATION_CONVERSION[t->Type][rotation];
      g->ScreenX = Constants::PREVIEW_POSITION_X + Constants::PREVIEW_BORDER +
                   Constants::TETROMINO_PREVIEW_OFFSET_COLUMN * Constants::TILE_WIDTH;
      g->ScreenY = Constants::PREVIEW_POSITION_Y + Constants::PREVIEW_BORDER +
                   Constants::TETROMINO_PREVIEW_OFFSET_ROW * Constants::TILE_HEIGHT;
    }
  }
}

bool GameLogicSystem::PlaceTetrominoOnBoard(uint32 tetromino) {
  // Get relevant data offsets
  uint32 bp_offset = m_entity_manager_->GetComponent<BoardPosition>(tetromino);
  uint32 t_offset = m_entity_manager_->GetComponent<Tetromino>(tetromino);
  uint32 gr_offset = m_entity_manager_->GetComponent<Graphics>(tetromino);

  if ((bp_offset != TetrisEntityManager::InvalidOffset) && (t_offset != TetrisEntityManager::InvalidOffset) &&
      (gr_offset != TetrisEntityManager::InvalidOffset)) {
    // Get the components
    BoardPosition* bp = m_entity_manager_->GetData<BoardPosition>() + bp_offset;
    Tetromino* t = m_entity_manager_->GetData<Tetromino>() + t_offset;
    Graphics* gr = m_entity_manager_->GetData<Graphics>() + gr_offset;

    // Get the positioning of the parts
    BoardLocation locations[4];
    TetrisHelper::GetBoardLocations(bp->Location, bp->Rotation, t->Type, locations);

    // Create new entities
    for (int32 part_index = 0;part_index < 4;++part_index) {
      uint32 current_part = m_entity_manager_->CreateEntity();

      uint32 part_bp_offset = m_entity_manager_->CreateComponent<BoardPosition>(current_part);
      BoardPosition* part_bp = m_entity_manager_->GetData<BoardPosition>() + part_bp_offset;
      part_bp->Location = locations[part_index];
      part_bp->Rotation = Constants::TILE_DEFAULT_ROTATION;

      uint32 part_gr_ofset = m_entity_manager_->CreateComponent<Graphics>(current_part);
      Graphics* part_gr = m_entity_manager_->GetData<Graphics>() + part_gr_ofset;
      part_gr->RepresentationAssetIdHash = gr->RepresentationAssetIdHash;

      m_entity_manager_->CreateComponent<Tile>(current_part);
    }

    // Destory this entity
    m_entity_manager_->DestroyEntity(tetromino);

    // Remove the lines
    RemoveLines();

    // Check if we can place a next piece
    if (CanPlaceNextTetrominoOnBoard()) {
      PlaceNextTetrominoOnBoard();
      GenerateNextTetromino();
      return true;
    } else {
      // Game over
      return false;
    }
  } else {
    // We got a weird entity - just fail silently
    return true;
  }
}

void GameLogicSystem::RemoveLines() {
  // Place tiles in a structure that reassembles the board
  uint32 row_entities[Constants::BOARD_HEIGHT][Constants::BOARD_WIDTH];
  uint16 row_counts[Constants::BOARD_HEIGHT];

  // Clear the tables
  for (int32 row_index = 0;row_index < Constants::BOARD_HEIGHT;++row_index) {
    row_counts[row_index] = 0;
  }

  // Do the placement
  uint32 tile_count = m_entity_manager_->GetDataCount<Tile>();
  const uint32* tile_entities = m_entity_manager_->GetDataEntities<Tile>();
  for (uint32 tile_index = 0;tile_index < tile_count;++tile_index) {
    uint32 bp_offset = m_entity_manager_->GetComponent<BoardPosition>(tile_entities[tile_index]);
    BoardPosition* bp = m_entity_manager_->GetData<BoardPosition>() + bp_offset;

    row_entities[bp->Location.row][row_counts[bp->Location.row]] = tile_entities[tile_index];
    row_counts[bp->Location.row]++;
  }

  // Move the lines and such
  BoardOffset offset = BoardOffset(0, 0);
  for (int32 row_index = Constants::BOARD_HEIGHT-1;row_index >= 0;--row_index) {
    if (row_counts[row_index] == Constants::BOARD_WIDTH) {
      for (int32 column_index = 0;column_index < row_counts[row_index];++column_index) {
        m_entity_manager_->DestroyEntity(row_entities[row_index][column_index]);
      }
      ++m_lines_;
      UpdateLevel();
      offset += BoardOffset(1, 0);
    } else {
      for (int32 column_index = 0;column_index < row_counts[row_index];++column_index) {
        uint32 current_tile = row_entities[row_index][column_index];
        uint32 bp_offset = m_entity_manager_->GetComponent<BoardPosition>(current_tile);
        BoardPosition* bp = m_entity_manager_->GetData<BoardPosition>() + bp_offset;
        bp->Location += offset;
      }
    }
  }
}

bool GameLogicSystem::CanPlaceNextTetrominoOnBoard() {
  // Get all tiles
  std::vector<BoardLocation*> tile_locations;

  // Get tile positions for testing
  GetTileLocations(&tile_locations);

  // Get the location of individual pieces of tetromino
  BoardLocation locations[4];
  BoardLocation base_location(Constants::TETROMINO_STARTING_ROW, Constants::TETROMINO_STARTING_COLUMN);
  uint32 t_offset = m_entity_manager_->GetComponent<Tetromino>(m_next_tetromino_);
  Tetromino* t = m_entity_manager_->GetData<Tetromino>() + t_offset;
  TetrisHelper::GetBoardLocations(base_location, Constants::TETROMINO_STARTING_ROTATION, t->Type, locations);

  // Test for collision
  return TetrisHelper::AreNotColiding(locations, tile_locations);
}

void GameLogicSystem::PlaceNextTetrominoOnBoard() {
  // Add movable component
  m_entity_manager_->CreateComponent<Movement>(m_next_tetromino_);

  // Add board position
  uint32 bp_offset = m_entity_manager_->CreateComponent<BoardPosition>(m_next_tetromino_);
  BoardPosition* bp = m_entity_manager_->GetData<BoardPosition>() + bp_offset;
  bp->Location.row = Constants::TETROMINO_STARTING_ROW;
  bp->Location.column = Constants::TETROMINO_STARTING_COLUMN;
  bp->Rotation = Constants::TETROMINO_STARTING_ROTATION;
}

void GameLogicSystem::GenerateNextTetromino() {
  // Generate a type and color
  int32 type_index = RandomInteger(0, 7);

  // Compute all the needed values
  TetrominoType type = Constants::TETROMINO_INDEX_TO_TETROMINO_TYPE[type_index];
  uint32 asset_id = Constants::TETROMINO_INDEX_TO_ASSET_ID[type_index];

  // Actually create a entity and its compoents
  m_next_tetromino_ = m_entity_manager_->CreateEntity();

  uint32 next_piece_graphics_offset = m_entity_manager_->CreateComponent<Graphics>(m_next_tetromino_);
  Graphics* next_piece_graphics = m_entity_manager_->GetData<Graphics>() + next_piece_graphics_offset;
  next_piece_graphics->RepresentationAssetIdHash = asset_id;

  uint32 next_piece_tetromino_offset = m_entity_manager_->CreateComponent<Tetromino>(m_next_tetromino_);
  Tetromino* next_piece_tetromino = m_entity_manager_->GetData<Tetromino>() + next_piece_tetromino_offset;
  next_piece_tetromino->Type = type;
}


void GameLogicSystem::UpdateLevel() {
  if (m_lines_ <= 0) {
    m_level_ = 1;
  } else if ((m_lines_ >= 1) && (m_lines_ <= 90)) {
    m_level_ = 1 + ((m_lines_ - 1) / 10);
  } else if (m_lines_ >= 91) {
    m_level_ = 10;
  }
  m_fall_period_ = GetFallPeriod(m_level_);
}

void GameLogicSystem::GetTileLocations(std::vector<BoardLocation*> *tile_locations) {
  uint32 entity_count = m_entity_manager_->GetDataCount<Tile>();
  const uint32* entities = m_entity_manager_->GetDataEntities<Tile>();

  for (uint32 tile_index = 0;tile_index < entity_count;++tile_index) {
    uint32 tile_bp_offset = m_entity_manager_->GetComponent<BoardPosition>(entities[tile_index]);
    BoardPosition* tile_bp = m_entity_manager_->GetData<BoardPosition>() + tile_bp_offset;
    tile_locations->push_back(&tile_bp->Location);
  }
}

int32 GameLogicSystem::RandomInteger(int32 min, int32 max) {
  // Supress the below warning in VS since rand is already thread safe according to MSDN
  return min + (std::rand() % static_cast<int32>(max - min));  // NOLINT(runtime/threadsafe_fn)
}

Elg::Clock::Milliseconds GameLogicSystem::GetFallPeriod(uint32 level) {
  return Elg::Clock::Milliseconds(50 * (11 - static_cast<uint64>(level)));
}
