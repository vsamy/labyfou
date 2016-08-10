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
  MenuState.cpp
  PauseState.cpp
  Player.cpp
  SceneNode.cpp
  SettingsState.cpp
  SpriteNode.cpp
  State.cpp
  TextNode.cpp
  World.cpp)

add_executable(labyfou ${HEADERS} ${SOURCES})
target_include_directories(labyfou PUBLIC ${CMAKE_SOURCE_DIR}/include)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(labyfou ${SFML_LIBRARIES})
endif()

install(TARGETS labyfou DESTINATION ${CMAKE_SOURCE_DIR})