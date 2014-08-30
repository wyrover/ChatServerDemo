{
  'target_defaults': {
  },
  'targets': [
    {
      'target_name': 'ChatServer',
      'type': 'executable',
      'sources': [
        './src/ChatRoom.cpp',
        './src/ChatServer.cpp',
        './src/ChatUser.cpp',
        './src/HttpClient.cpp',
        './src/main.cpp',
        './src/Session.cpp',
        './src/wapis.cpp',
        './src/WapisInvokeResult.cpp',
        './include/chat_server/ChatServerMessages.pb.cc',
        './deps/jsoncpp.cpp'
      ],
      'include_dirs': [
        './include',
        './deps'
      ],
      'conditions': [
        [
          'OS == "win"',
          {
            'defines': [ "_WIN32_WINNT=0x0501" ],
            'link_settings': {
              'libraries': [
                '-llibprotobuf-v120-md',
                '-llibcurl'
              ]
            }
          }
        ],
        [
          'OS == "mac"',
          {
          }
        ],
        [
          'OS == "linux"',
          {
            'cflags': [ "-std=c++11" ],
            'link_settings': {
              'libraries': [
                '-lprotobuf',
                '-lcurl',
                '-lboost_system'
              ]
            }
          }
        ]
      ],
      'defines': [
        'CHAT_SERVER_DEBUG',
        'CHAT_SERVER_LOGGER',
        '_ELPP_THREAD_SAFE'
      ],
      'msvs-settings': {
        'VCLinkerTool': {
          'SubSystem': 1,
        }
      }
    },
    {
      'target_name': 'ChatClient',
      'type': 'executable',
      'sources': [
        './tests/main.cpp',
        './tests/ChatClient.cpp',
        './src/Session.cpp',
        './include/chat_server/ChatServerMessages.pb.cc'
      ],
      'include_dirs': [
        './include',
        './deps'
      ],
      'conditions': [
        [
          'OS == "win"',
          {
            'defines': [ "_WIN32_WINNT=0x0501" ],
            'link_settings': {
              'libraries': [
                '-llibprotobuf-v120-md',
              ]
            }
          }
        ],
        [
          'OS == "mac"',
          {
          }
        ],
        [
          'OS == "linux"',
          {
            'cflags': [ "-std=c++11" ],
            'link_settings': {
              'libraries': [
                '-lprotobuf',
                '-lboost_system'
              ]
            }
          }
        ]
      ],
      'defines': [
        'CHAT_SERVER_LOGGER',
        '_ELPP_THREAD_SAFE'
      ],
      'msvs-settings': {
        'VCLinkerTool': {
          'SubSystem': 1,
        }
      }
    }
  ]
}
