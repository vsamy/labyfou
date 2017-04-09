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
add_executable(labyfou ${SOURCES})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(labyfou ${SFML_LIBRARIES})
endif()

install(TARGETS labyfou DESTINATION ${CMAKE_SOURCE_DIR})
set(INSTALL_PATH include/${PROJECT_NAME})
install(FILES ${HEADERS} DESTINATION ${INSTALL_PATH})
install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)