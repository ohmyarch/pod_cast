qt5_add_resources(QRC_FILE ../pod_cast.qrc)

add_executable(pod_cast
  main.cc
  ${QRC_FILE}
  )

target_link_libraries(pod_cast Qt5::QuickControls2)

install(TARGETS pod_cast RUNTIME DESTINATION bin)
