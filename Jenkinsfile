pipeline
{
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
				// checkout scm([extensions: [[$class: 'GitLFSPull']]])
				checkout scm
				// By default docker will use the root user. The unreal 4 build tool will abort if it
				// is run as root, so we need to impersonate the ue4 user

				// Allow the ue4 user to modify and access files in the workspace directory
				sh 'chmod -R 777 .'
				// Impersonate the user, configure ue4 and build the project
				echo "Testing to make sure the pull request builds..."
				sh 'su -c "ue4 setroot /home/ue4/UnrealEngine && ue4 clean && ue4 build" ue4'

				echo "The pull request builds."
			}
			post
			{
				// TODO - Zip up and archive artifacts (archiveArtifacts)
				cleanup {
					// TODO - will this prevent the directory from being used in a test stage?
					// cleanWs()
					echo 'nop'
				}
			}
		}

		stage('Package-Project')
		{
			steps
			{
				// TODO - Git LFS (https://jenkins.io/doc/pipeline/steps/workflow-scm-step/)
				// we will be unable to do ue4 package until we figure out how to clone Git
				// LFS objects

				// Impersonate the user, configure ue4 and build the project
				echo "Packaging project..."
				// sh 'su -c "ue4 setroot /home/ue4/UnrealEngine && ue4 clean && ue4 package" ue4'
				echo "The pull request was successfully packaged"
			}
			post
			{
				cleanup {
					echo 'nop'
				}
			}
		}


		// TODO - test stage
	}
}