TEMPLATE        = lib
TARGET		= aimsdata${BUILDMODEEXT}

#!include ../../config

INCBDIR = aims

win32:LIBS += -lregex

HEADERS=  bucket/bucket_g.h	\
    data/data_g.h		\
    border/border.h		\
    bucket/bucket.h		\
    bucket/bucketMap.h		\
    bucket/item.h		\
    bucket/sparsevolume.h       \
	bucket/bucketgen.h		\
    color/colormap.h		\
    config/aimsdata_config.h	\
    connectivity/connectivity.h \
    connectivity/connectivity_g.h \
    connectivity/component.h    \
    connectivity/component_d.h  \
    curve/curve.h		\
    data/cartodatavolume.h      \
    data/data.h			\
    data/fastAllocationData.h			\
    data/header.h		\
    data/pheader.h		\
    data/pheader_doc.h		\
    data/sliceformatheader.h	\
    data/volumemanip.h		\
    def/def_g.h			\
    def/assert.h 		\
    def/general.h		\
    def/info.h			\
    def/path.h			\
    def/settings.h		\
    doc/aimsdata_doc.h		\
    features/features.h \
    features/scalar_set_features.h \
    features/point_set_features.h \
    features/roi_features.h \
    getopt/getopt.h		\
    getopt/getopt2.h		\
    getopt/getoptProcess.h	\
    graph/graphiterator.h	\
    graph/graphmanip.h		\
    graph/graphmanip_d.h	\
    hsv/hsv.h                   \
    io/dbh.h			\
    io/aimsGraphR.h		\
    io/aimsGraphR_d.h		\
    io/aimsGraphW.h		\
    io/aimsGraphW_d.h		\
    io/apcreader.h		\
    io/argheader.h		\
    io/argR.h			\
    io/argW.h			\
    io/baseFormats_bucket.h	\
    io/baseFormats_bucket_d.h	\
    io/baseFormats_cartovolume.h \
    io/baseFormats_genericobject.h \
    io/baseFormats_graph.h	\
    io/baseFormats_hierarchy.h	\
    io/baseFormats_mesh.h	\
    io/baseFormats_mesh_d.h	\
    io/baseFormats_motion.h	\
    io/baseFormats_texture.h	\
    io/baseFormats_texture_d.h	\
    io/baseFormats_volume.h	\
    io/baseFormats_volume_d.h	\
    io/bckheader.h		\
    io/bckItemR.h		\
    io/bckItemW.h		\
    io/bckR.h			\
    io/bckW.h			\
    io/bckMapR.h		\
    io/bckMapW.h		\
    io/byteswap.h		\
    io/coarseR.h		\
    io/cmapR.h			\
    io/cmapW.h			\
    io/defaultItemR.h		\
    io/defaultItemW.h		\
    io/directory.h		\
    io/dtitensorItemR.h		\
    io/dtitensorItemW.h		\
    io/finder.h			\
    io/finderFormats.h		\
    io/fileFormat.h		\
    io/fileFormat_d.h		\
    io/fileFormat_cartovolume_d.h \
    io/ge_imageFileOffsets.h	\
    io/ge_imghdr.h		\
    io/genesisheader.h		\
    io/genesisR.h		\
    io/gisheader.h		\
    io/gisR.h			\
    io/gisW.h			\
    io/hieR.h			\
    io/io_g.h			\
    io/iooptions.h		\
    io/itemR.h			\
    io/itemR_stl.h		\
    io/itemW.h			\
    io/itemW_stl.h		\
    io/labelImage.h		\
    io/listItemR.h		\
    io/listItemW.h		\
    io/macosxbugs.h             \
    io/motionR.h		\
    io/motionW.h		\
    io/povW.h			\
    io/scaledcoding.h            \
    io/selectionr.h		\
    io/selectionw.h		\
    io/spmR.h			\
    io/spmR_d.h			\
    io/spmheader.h		\
    io/spmW.h			\
    io/spmW_d.h			\
    io/spmattribconverter.h     \
    io/triederItemR.h		\
    io/triederItemW.h		\
    io/trmheader.h		\
    io/reader.h			\
    io/reader_d.h		\
    io/readerasobject.h		\
    io/writer.h			\
    io/writer_d.h		\
    io/meshheader.h		\
    io/meshR.h			\
    io/meshW.h			\
    io/roiR.h			\
    io/tensorItemR.h    \
    io/tensorItemW.h    \
    io/triheader.h      \
    io/triR.h      \
    io/triW.h      \
    io/texheader.h      \
    io/texR.h      \
    io/texW.h      \
    io/process.h      \
    io/datatypecode.h    \
    math/dtitensor.h    \
    math/gausslu.h      \
    math/integrator.h    \
    math/lagrange.h      \
    math/mathelem.h      \
    math/romberg.h      \
    math/rombergitg.h    \
    math/tensor.h      \
    math/trapezeitg.h    \
    math/trieder.h      \
    memmap/allocator.h    \
    mesh/surface_g.h    \
    mesh/surface.h    \
    mesh/surfaceOperation.h  \
    mesh/surfaceOperation_d.h  \
    mesh/surfacegen.h	\
    mesh/texture.h      \
    mesh/tex2graph.h    \
    mesh/tex2graph_d.h    \
    meshprojection/meshprojection.h  \
    meshprojection/meshprojection_d.h  \
    resampling/mask.h \
    resampling/mask_d.h \
    resampling/motion.h    \
    resampling/linearInterpolator.h \
    resampling/quaternion.h \
    resampling/standardreferentials.h \
    rgb/rgb.h      \
    roi/hie.h      \
    roi/roi_g.h      \
    roi/maskIterator.h     \
    roi/roiIterator.h     \
    roi/roi.h      \
    roi/voxelSampling.h      \
    selection/selection.h    \
    selection/selector.h    \
    texdata/texdata.h    \
    utility/anytype_reader.h \
    utility/anytype_reader_details.h \
    utility/bininverse.h    \
    utility/boundingbox.h \
    utility/channel.h \
    utility/converter_g.h    \
    utility/converter_bucket.h  \
    utility/converter_hsv.h  \
    utility/converter_rgb.h  \
    utility/converter_texture.h  \
    utility/converter_volume.h  \
    utility/clock.h      \
    utility/flip.h      \
    utility/linearcomb.h    \
    utility/merge.h      \
    utility/minmax.h \
    utility/threshold.h    \
    utility/utility_g.h    \
    vector/vector.h

SOURCES = \
    bucket/bucketMap.cc \
	bucket/bucketgen.cc \
    border/border.cc	\
    color/colormap.cc		\
    connectivity/component.cc   \
    connectivity/connectivity.cc \
    curve/curve.cc		\
    data/cartodatavolume.cc     \
    data/data.cc     \
    data/fastAllocationData.cc			\
    data/header.cc		\
    data/pheader.cc		\
    data/sliceformatheader.cc	\
    def/filedim.cc		\
    def/general.cc		\
    def/info.cc			\
    def/path.cc			\
    def/settings.cc		\
    def/string.cc		\
    features/scalar_set_features.cc \
    features/point_set_features.cc \
    features/roi_features.cc \
    getopt/getopt.cc		\
    getopt/getopt2.cc		\
    graph/graphiterator.cc	\
    graph/graphmanip.cc		\
    io/aimsGraphR.cc		\
    io/aimsGraphW.cc		\
    io/apcreader.cc		\
    io/argheader.cc		\
    io/argR.cc			\
    io/argW.cc			\
    io/bckheader.cc		\
    io/coarseR.cc		\
    io/cmapR.cc			\
    io/cmapW.cc			\
    io/dtitensorItemR.cc	\
    io/dtitensorItemW.cc	\
    io/genesisheader.cc		\
    io/gisheader.cc		\
    io/hieR.cc			\
    io/iooptions.cc		\
    io/labelR.cc		\
    io/motionR.cc		\
    io/motionW.cc		\
    io/readerasobject.cc	\
    io/spmheader.cc		\
    io/spmR.cc			\
    io/spmW.cc			\
    io/spmattribconverter.cc    \
    io/texheader.cc		\
    io/triW.cc			\
    io/finder.cc		\
    io/finderFormats.cc    \
    io/triederItemR.cc    \
    io/triederItemW.cc    \
    io/tensorItemR.cc    \
    io/tensorItemW.cc    \
    io/reader.cc      \
    io/baseFormats_genericobject.cc \
    io/baseFormats_graph.cc    \
    io/baseFormats_mesh.cc	\
    io/baseFormats_motion.cc	\
    io/fileFormat.cc    \
    io/fileFormat_genericobject.cc \
    io/fileFormat_mesh.cc    \
    io/fileFormat_motion.cc    \
    io/fileFormat_texture.cc  \
    io/fileFormat_bucket.cc    \
    io/fileFormat_graph.cc    \
     io/baseFormats_hierarchy.cc  \
     io/fileFormat_hierarchy.cc  \
    io/fileFormat_volume.cc    \
    io/fileFormat_volumeVec.cc  \
    io/writer.cc      \
    io/meshheader.cc    \
    io/roiR.cc      \
    io/scaledcoding.cc \
    io/selectionr.cc \
    io/selectionw.cc \
    io/triheader.cc      \
    io/triR.cc      \
    io/trmheader.cc	\
    io/process.cc      \
    math/gausslu.cc      \
    math/mathelem.cc    \
    math/romberg.cc      \
    math/trapezeitg.cc    \
    math/triederRot.cc    \
    math/gccbuiltin.c		\
    memmap/allocator.cc    \
    mesh/surface.cc	\
    mesh/surfacegen.cc	\
    mesh/surfaceOperation.cc	\
    mesh/tex2graph.cc    \
    mesh/texture.cc    \
    meshprojection/meshprojection.cc    \
    resampling/mask.cc \
    resampling/motion.cc    \
    resampling/linearInterpolator.cc \
    resampling/quaternion.cc \
    resampling/standardreferentials.cc \
    roi/hie.cc      \
    roi/maskIterator.cc     \
    roi/roiIterator.cc     \
    roi/roi.cc      \
    roi/voxelSampling.cc      \
    selection/selection.cc	\
    selection/selector.cc	\
    utility/boundingbox.cc \
    utility/clock.cc \
    vector/vector.cc


