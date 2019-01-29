pipeline
{
	agent none
	
	stages
	{
		stage('Build')
		{
			parallel
			{
				stage('Linux Build')
				{
					agent
					{
						docker
						{
							image 'adamrehn/ue4-full:4.21.1'
						}
					}
					steps
					{
                        echo "Running ${env.BUILD_ID} on ${env.JENKINS_URL}"
                        echo "Cloning..."
                        checkout scm

						sh 'pwd'
                        sh 'ls'
					}
					post
					{
						cleanup {
							cleanWs()
						}
					}
				}
			}
		}
	}
}