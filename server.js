const express = require('express');
const { execFile } = require('child_process');
const path = require('path');

const app = express();
const PORT = process.env.PORT || 3000;

// Endpoint to run the C++ application
app.get('/run', (req, res) => {
    const binaryPath = path.join(__dirname, 'sNAKED.exe'); // Path to the compiled C++ binary

    execFile(binaryPath, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error: ${error.message}`);
            return res.status(500).send('Error executing the application.');
        }
        if (stderr) {
            console.error(`Stderr: ${stderr}`);
            return res.status(500).send('Application error.');
        }
        res.send(stdout); // Send the output of the C++ application
    });
});

app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});
