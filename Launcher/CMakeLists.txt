
set(MODULE_NAME Launcher)

add_executable( ${MODULE_NAME}
	Launcher.cpp
	Launcher.h
)


target_link_libraries(${MODULE_NAME} PUBLIC Engine)