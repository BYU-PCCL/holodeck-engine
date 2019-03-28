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
			image 'adamrehn/ue4-full:4.21.1'
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
				// 
				sh 'chmod 777 .'

				sh 'su ue4 Build/ContinuousIntegration/build_project.sh'

				echo "The pull request builds"
			}
		}
	    
		stage('Package-Project')
		{
			steps
			{
				echo "Cloning holodeck-worlds..."

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
				// Need to get permissions to move the holodeck-worlds repo
				sh 'chmod 777 .'				
				// Impersonate the user, configure ue4 and build the project
				sh 'su ue4 Build/ContinuousIntegration/package_project.sh'
								
				// Pull down the latest build of holodeck-worlds
				echo "The pull request was successfully packaged"
			}
			post
			{
				success {
					sh "tar -zcvf holodeck-engine.tar.gz dist/"
					archiveArtifacts artifacts:'holodeck-engine.tar.gz', fingerprint: true
				}
			}
		}
	}
}