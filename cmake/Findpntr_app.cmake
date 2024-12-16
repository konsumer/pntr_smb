include(FetchContent)

# FetchContent_Declare(pntr_app
#     URL "https://github.com/robloach/pntr_app/archive/refs/heads/master.zip"
# )


FetchContent_Declare(
    pntr_app
    GIT_REPOSITORY https://github.com/robloach/pntr_app.git
    GIT_TAG 2b1c6fd8d5
)

FetchContent_MakeAvailable(pntr_app)
