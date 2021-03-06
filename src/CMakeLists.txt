qt5_add_resources(QRC_FILE ../pod_cast.qrc)

qt5_wrap_cpp(PODCAST_MODEL_MOC ../include/PodcastModel.h)

if(WIN32)
  add_executable(pod_cast
    WIN32
    main.cc
    PodcastModel.cc
    ../pod_cast.rc
    ${PODCAST_MODEL_MOC}
    ${QRC_FILE}
  )
else()
  add_executable(pod_cast
    main.cc
    PodcastModel.cc
    ${PODCAST_MODEL_MOC}
    ${QRC_FILE}
  )
endif()

if(AUTO_REFORMAT_QML_FILES)
  add_dependencies(pod_cast reformat_qml_files)
endif()

if(IOS OR ANDROID)
  target_link_libraries(pod_cast
    feedparser
    Qt5::WebView
    Qt5::QuickControls2
  )
elseif(WIN32)
  target_link_libraries(pod_cast
    feedparser
    Qt5::Svg
    Qt5::WebView
    Qt5::Widgets
    Qt5::QuickControls2
  )
else()
  target_link_libraries(pod_cast
    feedparser
    Qt5::WebView
    Qt5::Widgets
    Qt5::QuickControls2
  )
endif()

install(TARGETS pod_cast RUNTIME DESTINATION bin)
