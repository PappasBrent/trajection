target_sources(${PROJECT_NAME} PRIVATE
    cannon.c
    direction.c
    globals.c
    level.c
    main.c
    turn_circle.c)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

target_sources(${PROJECT_NAME}-editor PRIVATE
    cannon.c
    direction.c
    globals.c
    level.c
    editor.c
    turn_circle.c)
target_link_libraries(${PROJECT_NAME}-editor PRIVATE raylib)
