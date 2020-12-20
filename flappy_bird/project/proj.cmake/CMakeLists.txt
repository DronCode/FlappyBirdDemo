cmake_minimum_required (VERSION 2.6)
project (flappy_bird)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(../../../libs/oxygine-framework/ oxygine-framework)
add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})


add_subdirectory(../../../libs/oxygine-sound/ oxygine-sound)
add_definitions(${OXYGINESOUND_DEFINITIONS})
include_directories(${OXYGINESOUND_INCLUDE_DIRS})
link_directories(${OXYGINESOUND_LIBRARY_DIRS})

add_subdirectory(../../../libs/oxygine-flow/ oxygine-flow)
include_directories(${OXYGINE_FLOW_INCLUDE_DIRS})

add_subdirectory(../../../libs/box2d box2d)

add_subdirectory(../../../libs/oxygine-freetype/ oxygine-freetype)
include_directories(${OXYGINE_FREETYPE_INCLUDE_DIRS})
link_directories(${OXYGINE_FREETYPE_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS} ${OXYGINE_FREETYPE_CXX_FLAGS}")

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
set(SOURCES
	${SRC}/Game.cpp
	${SRC}/GameScene.cpp
	${SRC}/MainMenuScene.cpp
	${SRC}/MyButton.cpp
	${SRC}/MyScene.cpp
	${SRC}/FlappyBirdBootstrap.cpp
	${SRC}/FlappyBirdBootstrap.h
	${SRC}/main.cpp
	${SRC}/res.cpp
	${SRC}/Game.h
	${SRC}/GameScene.h
	${SRC}/MainMenuScene.h
	${SRC}/MyButton.h
	${SRC}/MyScene.h
	${SRC}/res.h
	${SRC}/Player.h
	${SRC}/Player.cpp
	${SRC}/PhyData.h
	${SRC}/GamePhysicsWorldContactProcessor.h
	${SRC}/GamePhysicsWorldContactProcessor.cpp
	${SRC}/DeadZone.h
	${SRC}/DeadZone.cpp
	${SRC}/Box2DDebugDraw.cpp
	${SRC}/Box2DDebugDraw.h
	${SRC}/GlobalSettings.h
	${SRC}/PhysicsBasedActor.h
	${SRC}/PhysicsBasedActor.cpp
	${SRC}/Obstacle.h
	${SRC}/Obstacle.cpp
	${SRC}/ObstacleController.h
	${SRC}/ObstacleController.cpp
	${SRC}/ObstacleGroupDefinition.cpp
	${SRC}/SafeAreaTrigger.h
	${SRC}/SafeAreaTrigger.cpp
	${SRC}/GameplaySession.h
	${SRC}/GameplaySession.cpp
	${SRC}/GameResultDialog.h
	${SRC}/GameResultDialog.cpp
	${SRC}/GameFinishedEvent.h
	${SRC}/GameFinishedEvent.cpp
	${SRC}/Random.hpp
)

add_executable(flappy_bird ${SOURCES})

if (WIN32) #disable console mode for VC++
	set_target_properties(flappy_bird PROPERTIES WIN32_EXECUTABLE TRUE)
endif(WIN32)

target_link_libraries(flappy_bird ${OXYGINESOUND_LIBS} oxygine-flow ${OXYGINE_CORE_LIBS} ${OXYGINE_FREETYPE_LIBS} oxygine-freetype box2d)

if (EMSCRIPTEN)
	SET(CMAKE_EXECUTABLE_SUFFIX ".html")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s NO_EXIT_RUNTIME=1 -s WARN_ON_UNDEFINED_SYMBOLS=1 --memory-init-file 0 -s TOTAL_MEMORY=50331648 -s FORCE_FILESYSTEM=1")
	em_link_pre_js(flappy_bird  ${OXYGINE_JS_LIBRARIES}  ${CMAKE_CURRENT_SOURCE_DIR}/data.js)
endif(EMSCRIPTEN)