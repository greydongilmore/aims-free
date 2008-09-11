
#numtypes = [ 'unsigned char', 'short', 'unsigned short', 'int',
             #'unsigned', 'float', 'double' ]
#basetypes = numtypes + [ 'AimsRGB', 'AimsRGBA' ]
basetypes = [ 'short', 'float' ]
matrix = []
for z in [ map( lambda y: (x,y), basetypes ) for x in basetypes ]:
  matrix += z

todo = { 'system' : [ 'Void' ], 
         'volume' : basetypes,
         'aimsdata' : basetypes,
         'vector' : [ 
                      'int', 
                      #'unsigned', 
                      'float', 
                      #'double', 
                      #'Point2df', 
                      #'Point3df', 
                      #'std::string', 
                      #'AimsVector<unsigned,2>',
                      #'AimsVector<unsigned,3>', 'AimsVector<unsigned,4>',
                      #'Point3d', 
                    ],
         'list' :   [ 
                      'int', 
                      #'unsigned', 
                      'float', 
                      #'double', 
                      #'std::string' 
                    ],
         'set' :    [ 
                      'int', 
                      #'unsigned', 
                      'float', 
                      #'double', 
                      #'std::string', 
                      #'Vertex *', 
                      #'Edge *'
                     ],
         'map' :    [ ( 'std::string', x ) \
                      for x in basetypes + \
                      [ 'std::string', 'carto::Semantic' ] ],
         'rcptr' : [ 
                     #'AimsSurfaceTriangle', 
                     'carto::GenericObject',
                     'AimsData<int16_t>', 'AimsData<float>',
                     'carto::Volume<int16_t>', 'carto::Volume<float>',
                     #'AimsData<uint8_t>', 'AimsData<int16_t>',
                     #'AimsData<uint16_t>', 'AimsData<int32_t>',
                     #'AimsData<uint32_t>', 'AimsData<float>',
                     #'AimsData<double>', 'AimsData<AimsRGB>',
                     #'AimsData<AimsRGBA>',
                     #'carto::Volume<uint8_t>', 
                     #'carto::Volume<int16_t>', 'carto::Volume<uint16_t>', 
                     #'carto::Volume<int32_t>', 'carto::Volume<uint32_t>', 
                     #'carto::Volume<float>', 'carto::Volume<double>', 
                     #'carto::Volume<AimsRGB>', 'carto::Volume<AimsRGBA>', 
                     #'AimsTimeSurface<2,Void>',
                     #'AimsTimeSurface<3,Void>', 'AimsTimeSurface<4,Void>',
                     #'TimeTexture<float>', 'TimeTexture<int16_t>',
                     #'TimeTexture<int32_t>', 'TimeTexture<uint32_t>',
                     #'TimeTexture<Point2df>', 
                     #'aims::BucketMap<Void>',
                     #'aims::BucketMap<int16_t>', 'aims::BucketMap<float>',
                     'Motion', 
                     #'carto::Site', 
                     #'carto::SiteIterator', 
                     ], 
#         'converter_Volume' : matrix,
         'reader' : [ 
                      #'Graph', 'AimsSurfaceTriangle',
                      'AimsData<int16_t>', 'AimsData<float>',
                      'carto::Volume<int16_t>', 'carto::Volume<float>', 
                      #'AimsData<uint8_t>', 'AimsData<int16_t>',
                      #'AimsData<uint16_t>', 'AimsData<int32_t>',
                      #'AimsData<uint32_t>', 'AimsData<float>',
                      #'AimsData<double>', 'AimsData<AimsRGB>',
                      #'AimsData<AimsRGBA>', 
                      #'carto::Volume<uint8_t>', 'carto::Volume<int16_t>', 
                      #'carto::Volume<uint16_t>', 'carto::Volume<int32_t>', 
                      #'carto::Volume<uint32_t>', 'carto::Volume<float>', 
                      #'carto::Volume<double>', 'carto::Volume<AimsRGB>', 
                      #'carto::Volume<AimsRGBA>',
                      #'AimsTimeSurface<2,Void>', 
                      #'AimsTimeSurface<3,Void>', 'AimsTimeSurface<4,Void>',
                      #'TimeTexture<float>', 'TimeTexture<int16_t>',
                      #'TimeTexture<int32_t>', 'TimeTexture<uint32_t>',
                      #'TimeTexture<Point2df>', 'aims::BucketMap<Void>',
                      'Motion', 
                      ],
         #'aimstimesurface' : [ '2', '3', '4', ],
         #'timetexture' : [ 'float', 'short', 'int', 'unsigned', 'Point2df' ],
         'bucketmap' : [ 'Void', 'short', 'float' ],
         'aimsvector' : [ ( 'float', '2' ), ( 'float', '3' ),( 'float', '4' ),
                          ( 'short', '3' ),
                          #( 'unsigned', '2' ), ( 'unsigned', '3' ),
                          #( 'unsigned', '4' ), 
                          ], 
         'threshold' : [ ( 'float', 'short' ) ],
         }

