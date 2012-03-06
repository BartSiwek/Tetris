/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include "Tetris/Systems/UserInputSystem/UserInputSystem.h"

#include <SDL/SDL.h>

#include <algorithm>
#include <iterator>
#include <vector>

#include "Elg/Clock/TimePoint.h"
#include "Elg/Clock/GameClock.h"
#include "Elg/EntityManager/EntityManager.h"

#include "Tetris/Helpers/TetrisHelper.h"
#include "Tetris/Helpers/Constants.h"
#include "Tetris/Systems/UserInputSystem/UserInputSystemProfiling.h"

UserInputSystem::UserInputSystem(TetrisEntityManager *entity_manager) : m_entity_manager_(entity_manager) {
}

UserInputSystem::~UserInputSystem() {
}

void UserInputSystem::Initialize() {
  SDL_EnableKeyRepeat(Constants::KEYBOARD_REPEAT_DELAY, Constants::KEYBOARD_REPEAT_INTERVAL);
}

bool UserInputSystem::Update(Elg::Clock::GameClock::TimePoint /* time */, Elg::Clock::GameClock::Duration /* dt */) {
  TETRIS_PROFILE_USERINPUTSYSTEM_UPDATE;

  // A Structure of arrays event queue
  TetrisEvent events[Constants::MAX_INPUT_EVENTS];
  uint32 event_count = 0;

  // Poll for events and queue the ones we care about - limit it to MAX_EVENTS
  SDL_Event event;
  bool should_continue = true;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        should_continue = false;
      case SDL_KEYDOWN:
        should_continue = should_continue && ProcessKeydownEvent(event, &event_count, events);
    }
  }

  // If we got some reasonable events process them
  ProcessEvents(event_count, events);

  return should_continue;
}

bool UserInputSystem::ProcessKeydownEvent(const SDL_Event &event, uint32 *event_count, TetrisEvent *events) {
  uint32 current_event_count = *event_count;
  if (current_event_count < Constants::MAX_INPUT_EVENTS) {
    if (event.key.keysym.sym == SDLK_LEFT) {
      events[current_event_count] = MOVE_LEFT;
      ++current_event_count;
    } else if (event.key.keysym.sym == SDLK_RIGHT) {
      events[current_event_count] = MOVE_RIGHT;
      ++current_event_count;
    } else if (event.key.keysym.sym == SDLK_DOWN) {
      events[current_event_count] = DROP;
      ++current_event_count;
    } else if (event.key.keysym.sym == SDLK_SPACE) {
      events[current_event_count] = ROTATE;
      ++current_event_count;
    }
    *event_count = current_event_count;
  }

  return true;
}

void UserInputSystem::ProcessEvents(const uint32 &event_count, const TetrisEvent *events) {
  // Movable pieces have MovementComponent
  Movement* movement_component = m_entity_manager_->GetData<Movement>();
  Movement* movement_component_end = movement_component + m_entity_manager_->GetDataCount<Movement>();

  while (movement_component != movement_component_end) {
    movement_component->EventCount = 0;

    const TetrisEvent* current_event = events;
    const TetrisEvent* current_event_end = events + event_count;
    while (current_event != current_event_end) {
      movement_component->Events[movement_component->EventCount++] = *current_event;
      ++current_event;
    }

    ++movement_component;
  }
}
