pipeline
{
	options {
		// Only keep 15 latest build artifacts, build logs for 60 days
		// TODO: Will we want Jenkins to keep some builds longer?
    	buildDiscarder(logRotator(daysToKeepStr: '60', artifactNumToKeepStr: '15'))
  	}
	// Use one agent for everything. When we have Windows builds, this will have to change
	agent
	{
		docker 
		{
			image 'adamrehn/ue4-full:4.22.3'
			// TODO: persist in volume? Does Jenkins use a bind mount?
		}
	}
	
	stages
	{
		// TODO - add Windows agents/builds
		stage('Build-Editor')
		{
			steps
			{
				// The UnrealEditorTool refuses to run as root, even in a docker container.

				sh 'chmod 777 .'

				sh 'su ue4 Build/ContinuousIntegration/build_project.sh'

				echo "The pull request builds"
			}
		}
	    
		stage('Package-Project')
		{
			steps
			{
				dir('holodeck-worlds') {
					checkout ([
						userRemoteConfigs: [[
							url: 'https://github.com/BYU-PCCL/holodeck-worlds',
							credentialsId: 'github'
						]],
						$class: 'GitSCM',
						branches: [[name: '*/master']],
						extensions: [[$class: 'GitLFSPull'],
						             [$class: 'CloneOption',
                    					depth: 0,
                    					noTags: false,
                    					shallow: true,
                    					timeout: 120]
						]
					])
				}

				dir('holodeck-configs') {
					checkout ([
						userRemoteConfigs: [[
							url: 'https://github.com/BYU-PCCL/holodeck-configs',
							credentialsId: 'github'
						]],
						$class: 'GitSCM',
						branches: [[name: '*/master']],
						extensions: [[$class: 'CloneOption',
                    					depth: 0,
                    					noTags: false,
                    					shallow: true,
                    					timeout: 120]
						]
					])

				}
				// Need to get permissions to move the holodeck-worlds repo
				sh 'chmod 777 -R .'				
				// Impersonate the user, configure ue4 and build the project
				sh 'su ue4 Build/ContinuousIntegration/package_ue4_project.sh'
								
				// Pull down the latest build of holodeck-worlds
				echo "The pull request was successfully packaged"
			}
			post
			{
				success {
					archiveArtifacts artifacts:'*.zip', fingerprint: true
				}
				cleanup {
					cleanWs()
				}
			}
		}
	}
}
