
numtypes = [ 'unsigned char', 'short', 'unsigned short', 'int',
             'unsigned', 'float', 'double' ]
basetypes = numtypes + [ 'AimsRGB', 'AimsRGBA' ]
matrix = []
for z in [ map( lambda y: (x,y), basetypes ) for x in basetypes ]:
  matrix += z

todo = { 'system' : [ 'Void' ], 
         'volume' : basetypes,
         'aimsdata' : basetypes,
         'vector' : [ 'short', 'int', 'unsigned', 'float', 'double',
                      'Point2df', 'Point3df', 'Point3dd', 'Point3d',
                      'std::string', 'AimsVector<unsigned,2>',
                      'AimsVector<unsigned,3>', 'AimsVector<unsigned,4>' ],
         'list' :   [ 'int', 'unsigned', 'float', 'double', 'std::string' ],
         'set' :    [ 'short', 'int', 'unsigned', 'float', 'double',
                      'std::string',
                      'Vertex *', 'Edge *' ],
         'map' :    [ ( 'std::string', x ) \
                      for x in basetypes + \
                      [ 'std::string', 'carto::Semantic' ]] +
                      [('int', 'std::string')],
         'rcptr' : [ 'AimsSurfaceTriangle', 'carto::GenericObject',
                     'AimsData<uint8_t>', 'AimsData<int16_t>',
                     'AimsData<uint16_t>', 'AimsData<int32_t>',
                     'AimsData<uint32_t>', 'AimsData<float>',
                     'AimsData<double>', 'AimsData<AimsRGB>',
                     'AimsData<AimsRGBA>',
                     'carto::Volume<uint8_t>', 
                     'carto::Volume<int16_t>', 'carto::Volume<uint16_t>', 
                     'carto::Volume<int32_t>', 'carto::Volume<uint32_t>', 
                     'carto::Volume<float>', 'carto::Volume<double>', 
                     'carto::Volume<AimsRGB>', 'carto::Volume<AimsRGBA>', 
                     'AimsTimeSurface<2,Void>',
                     'AimsTimeSurface<3,Void>', 'AimsTimeSurface<4,Void>',
                     'TimeTexture<float>', 'TimeTexture<int16_t>',
                     'TimeTexture<int32_t>', 'TimeTexture<uint32_t>',
                     'TimeTexture<Point2df>', 'aims::BucketMap<Void>',
                     'aims::BucketMap<int16_t>', 'aims::BucketMap<float>',
                     'Motion',
                     'carto::Site', 
                     'carto::SiteIterator',
                     'aims::Interpolator',
                     ], 
         'converter_Volume' : matrix,
         'converter' : [ ( 'AimsData<uint8_t>', 'aims::BucketMap<Void>' ),
                         ( 'AimsData<uint8_t>', 'aims::BucketMap<float>' ),
                         ( 'AimsData<uint16_t>', 'aims::BucketMap<Void>' ),
                         ( 'AimsData<uint16_t>', 'aims::BucketMap<float>' ),
                         ( 'AimsData<int16_t>', 'aims::BucketMap<Void>' ),
                         ( 'AimsData<int16_t>', 'aims::BucketMap<int16_t>' ),
                         ( 'AimsData<int16_t>', 'aims::BucketMap<float>' ),
                         ( 'AimsData<uint32_t>', 'aims::BucketMap<Void>' ),
                         ( 'AimsData<uint32_t>', 'aims::BucketMap<float>' ),
                         ( 'AimsData<int32_t>', 'aims::BucketMap<Void>' ),
                         ( 'AimsData<int32_t>', 'aims::BucketMap<float>' ),
                         ( 'AimsData<float>', 'aims::BucketMap<Void>' ),
                         ( 'AimsData<float>', 'aims::BucketMap<float>' ),
                         ( 'AimsData<double>', 'aims::BucketMap<Void>' ),
                         ( 'AimsData<double>', 'aims::BucketMap<float>' ),
                         ( 'aims::BucketMap<Void>', 'AimsData<uint8_t>' ),
                         ( 'aims::BucketMap<Void>', 'AimsData<uint16_t>' ),
                         ( 'aims::BucketMap<Void>', 'AimsData<int16_t>' ),
                         ( 'aims::BucketMap<Void>', 'AimsData<uint32_t>' ),
                         ( 'aims::BucketMap<Void>', 'AimsData<int32_t>' ),
                         ( 'aims::BucketMap<Void>', 'AimsData<float>' ),
                         ( 'aims::BucketMap<Void>', 'AimsData<double>' ),
                         ( 'aims::BucketMap<int16_t>', 'AimsData<uint8_t>' ),
                         ( 'aims::BucketMap<int16_t>', 'AimsData<uint16_t>' ),
                         ( 'aims::BucketMap<int16_t>', 'AimsData<int16_t>' ),
                         ( 'aims::BucketMap<int16_t>', 'AimsData<uint32_t>' ),
                         ( 'aims::BucketMap<int16_t>', 'AimsData<int32_t>' ),
                         ( 'aims::BucketMap<int16_t>', 'AimsData<float>' ),
                         ( 'aims::BucketMap<int16_t>', 'AimsData<double>' ),
                         ( 'aims::BucketMap<float>', 'AimsData<uint8_t>' ),
                         ( 'aims::BucketMap<float>', 'AimsData<uint16_t>' ),
                         ( 'aims::BucketMap<float>', 'AimsData<int16_t>' ),
                         ( 'aims::BucketMap<float>', 'AimsData<uint32_t>' ),
                         ( 'aims::BucketMap<float>', 'AimsData<int32_t>' ),
                         ( 'aims::BucketMap<float>', 'AimsData<float>' ),
                         ( 'aims::BucketMap<float>', 'AimsData<double>' ),
                       ],
         'reader' : [ 'Graph', 'AimsSurfaceTriangle',
                      'AimsData<uint8_t>', 'AimsData<int16_t>',
                      'AimsData<uint16_t>', 'AimsData<int32_t>',
                      'AimsData<uint32_t>', 'AimsData<float>',
                      'AimsData<double>', 'AimsData<AimsRGB>',
                      'AimsData<AimsRGBA>', 
                      'carto::Volume<uint8_t>', 'carto::Volume<int16_t>', 
                      'carto::Volume<uint16_t>', 'carto::Volume<int32_t>', 
                      'carto::Volume<uint32_t>', 'carto::Volume<float>', 
                      'carto::Volume<double>', 'carto::Volume<AimsRGB>', 
                      'carto::Volume<AimsRGBA>', 'AimsTimeSurface<2,Void>', 
                      'AimsTimeSurface<3,Void>', 'AimsTimeSurface<4,Void>',
                      'TimeTexture<float>', 'TimeTexture<int16_t>',
                      'TimeTexture<int32_t>', 'TimeTexture<uint32_t>',
                      'TimeTexture<Point2df>',
                      'aims::BucketMap<Void>', 'aims::BucketMap<int16_t>',
                      'aims::BucketMap<float>',
                      'Motion', 'aims::Hierarchy', 'carto::Object',
                      ],
         'aimstimesurface' : [ '2', '3', '4', ],
         'timetexture' : [ 'float', 'short', 'int', 'unsigned', 'Point2df' ],
         'bucketmap' : [ 'Void', 'short', 'float' ],
         'aimsvector' : [ ( 'float', '2' ), ( 'float', '3' ),
                          ( 'float', '4' ), ( 'double', '3' ),
                          ( 'short', '3' ),
                          ( 'unsigned', '2' ), ( 'unsigned', '3' ),
                          ( 'unsigned', '4' ),
                          ], 
         'threshold' : [ ( 'float', 'short' ), ( 'short', 'short') ],
         'merge' : [ ( 'short', 'short'), ],
         }

