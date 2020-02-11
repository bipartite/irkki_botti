# Match all files, except hidden ones (which begin with dot).
file(GLOB DATA_FILES "share/[^.]*")

if(DATA_FILES)
	install(FILES ${DATA_FILES} DESTINATION "${PROJECT_DATA_DIR}")
endif(DATA_FILES)

