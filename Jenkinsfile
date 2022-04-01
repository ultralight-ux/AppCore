pipeline {
  agent none
  stages {
    stage('Build') {
      parallel {
        stage('Build macOS') {
          agent {
            node {
              label 'macos'
            }
          }
          steps {
            sh '''
               # Setup environment
               export PATH="/usr/local/bin:$PATH"

               # Build Release
               mkdir -p build
               cd build
               cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DUL_GENERATE_SDK=1
               ninja
               cd ..
            '''
          }
          post {
            success {
              deploy();
            }
          }
        }
        stage('Build Windows x64') {
          agent {
            node {
              label 'win_x64'
            }
          }
          steps {
            bat '''
               rem Setup environment
               call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" amd64
               set CC=cl.exe
               set CXX=cl.exe

               rem Build Release
               if not exist build mkdir build
               cd build
               cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DUL_GENERATE_SDK=1
               ninja
            '''
          }
          post {
            success {
              deploy();
            }
          }
        }
        stage('Build Linux') {
          agent {
            node {
              label 'linux'
            }
          }
          steps {
            sh '''     
               # Build Release
               mkdir -p build
               cd build
               export CC=/usr/bin/clang
               export CXX=/usr/bin/clang++
               cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DUL_GENERATE_SDK=1
               ninja
               cd ..
            '''
          }
          post {
            success {
              deploy();
            }
          }
        }
      }
    }
  }
}

def deploy() {
  withAWS(endpointUrl:'https://sfo2.digitaloceanspaces.com', credentials:'jenkins-access') {
    if (env.BRANCH_NAME == 'master') {
      s3Upload(bucket: 'appcore-bin', workingDir:'build', includePathPattern:'*.7z', acl:'PublicRead');
    } else if (env.BRANCH_NAME == 'dev') {
      s3Upload(bucket: 'appcore-bin-dev', workingDir:'build', includePathPattern:'*.7z', acl:'PublicRead');
    }
  }
}