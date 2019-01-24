add_executable(frontback split_frontback/frontback.cpp)
set_target_properties(frontback PROPERTIES FOLDER tests/split_frontback)

add_executable(Mog2 split_frontback/Mog2.cpp)
set_target_properties(Mog2 PROPERTIES FOLDER tests/split_frontback)

add_executable(bgfg_segm split_frontback/bgfg_segm.cpp)
set_target_properties(bgfg_segm PROPERTIES FOLDER tests/split_frontback)

add_executable(testCmdLine testcmdline.cpp)
set_target_properties(testCmdLine PROPERTIES FOLDER tests)

add_executable(grabcut split_frontback/grabcut.cpp)
set_target_properties(grabcut PROPERTIES FOLDER tests/split_frontback)

# auto matting 
ADD_EXECUTABLE(auto_matting split_frontback/auto_matting.cpp)
SET_TARGET_PROPERTIES(auto_matting PROPERTIES FOLDER tests/split_frontback)
#TARGET_LINK_LIBRARIES(auto_matting opencv_photo320.lib)


# blend
# 泊松融合 possion matting
ADD_EXECUTABLE(possion_matting split_frontback/possion_matting.cpp)
SET_TARGET_PROPERTIES(possion_matting PROPERTIES FOLDER tests/split_frontback)
TARGET_LINK_LIBRARIES(possion_matting opencv_photo320.lib)

# 拉普拉斯融合laplacian blending 
ADD_EXECUTABLE(laplacian_blend split_frontback/laplacian_blend.cpp)
SET_TARGET_PROPERTIES(laplacian_blend PROPERTIES FOLDER tests/split_frontback)
TARGET_LINK_LIBRARIES(laplacian_blend opencv_photo320.lib)
