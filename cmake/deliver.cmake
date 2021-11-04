add_custom_target("pkg"
    COMMENT "Packaging for deliverable on moodle"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../
    COMMAND [ -d fern ] && rm -rf fern* || echo "No old package..."
    COMMAND cp -r ${PROJECT_SOURCE_DIR} fern
    COMMAND rm -rf fern/build/ && rm -rf fern/.git*
    COMMAND [ -d fern/.cache ] && rm -rf fern/.cache || echo "No cache..."
    COMMAND zip -r fern.zip fern
)

add_custom_target("server"
    COMMENT "Packaging for testing on server"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../
    DEPENDS pkg
    COMMAND scp -r fern.zip ga74ped@hpc04.clients.eikon.tum.de:/home/ga74ped/Documents/
)

add_custom_target("result"
    COMMENT "Taking final image from server"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../
    COMMAND scp ga74ped@hpc04.clients.eikon.tum.de:/home/ga74ped/Documents/fern/${PROJECT_NAME}.png ~/Downloads/fern_server.png
)
