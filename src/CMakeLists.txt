set(SOURCES
  Application.cpp
  Button.cpp
  Character.cpp
  Command.cpp
  Component.cpp
  Container.cpp
  DataTables.cpp
  Entity.cpp
  GameState.cpp
  Label.cpp
  main.cpp
  mathFunctions.cpp
  Maze.cpp
  MazeGenerator.cpp
  MenuState.cpp
  PauseState.cpp
  Player.cpp
  SceneNode.cpp
  SettingsState.cpp
  SpriteNode.cpp
  State.cpp
  TextNode.cpp
  World.cpp)

include_directories(../include)
include_directories(${SFML_INCLUDE_DIR})
add_executable(labyfou ${SOURCES})
target_link_libraries(labyfou ${SFML_LIBRARIES})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)