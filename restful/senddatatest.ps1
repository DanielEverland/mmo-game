$server = "127.0.0.1"  # Replace with your server's IP or hostname
$port = 8080           # Replace with your server's port
$message = "GET / HTTP/1.1`r`nHost: 127.0.0.1`r`nConnection: close`r`n`r`n"

# Create a TCP connection
$client = New-Object System.Net.Sockets.TcpClient($server, $port)

# Get the network stream
$stream = $client.GetStream()

Write-Output "Sending"

# Send data to the server
$writer = New-Object System.IO.StreamWriter($stream)
$writer.Write($message)
$writer.Flush()

Write-Output "Listening for response"

# Receive the response
$reader = New-Object System.IO.StreamReader($stream)
$response = $reader.ReadToEnd()

# Output the response
Write-Output $response

# Clean up
$reader.Close()
$writer.Close()
$stream.Close()
$client.Close()
