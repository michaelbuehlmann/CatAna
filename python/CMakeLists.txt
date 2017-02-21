pybind11_add_module(pybasictypes py_basictypes.cpp)
set_target_properties(pybasictypes PROPERTIES OUTPUT_NAME basictypes)

pybind11_add_module(pybesseltools py_besseltools.cpp)
set_target_properties(pybesseltools PROPERTIES OUTPUT_NAME besseltools)

pybind11_add_module(pydecomposition py_decomposition.cpp)
set_target_properties(pydecomposition PROPERTIES OUTPUT_NAME decomposition)

pybind11_add_module(pyio py_io.cpp)
set_target_properties(pyio PROPERTIES OUTPUT_NAME io)


target_link_libraries(pybasictypes PRIVATE catana)
target_link_libraries(pybesseltools PRIVATE catana)
target_link_libraries(pydecomposition PRIVATE catana)
target_link_libraries(pyio PRIVATE catana)