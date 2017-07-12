
#TODO we need to add the error message (if applicable) to the output
function Process-Tests {
    process {
        $test_status = "None"
        if($_.status -eq "run") {
            if($_.failure) {
                $test_status = "Failed"
            } else {
                $test_status = "Passed"
            }
        } elseif ($_.status -eq "notrun"){
			$test_status = "Skipped"
		}
        New-Object psobject -Property @{
            'name' = $_.ParentNode.name + "." + $_.name;
            'status' = $test_status;
            'time' = [long]([float]$_.time * 1000);
			'classname' = $_.classname
        }
    }
}

# Writes test results to appveyor dashboard
function Upload-TestResults {
	process {
		Add-AppveyorTest -Name $_.name -Framework "xUnit" -FileName $_.classname -Outcome $_.status -Duration $_.time
	}
}

$testResults = Select-Xml -Path "$env:TEST_RESULT_DIR\*.xml" //testcase | Select-Object -ExpandProperty Node | Process-Tests | Upload-TestResults
