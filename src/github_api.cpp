/*
 Copyright (c) 2023 Miguel Tomas, http://www.aeonlabs.science

License Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
You are free to:
   Share — copy and redistribute the material in any medium or format
   Adapt — remix, transform, and build upon the material

The licensor cannot revoke these freedoms as long as you follow the license terms. Under the following terms:
Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. 
You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.

NonCommercial — You may not use the material for commercial purposes.

ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under
 the same license as the original.

No additional restrictions — You may not apply legal terms or technological measures that legally restrict others
 from doing anything the license permits.

Notices:
You do not have to comply with the license for elements of the material in the public domain or where your use
 is permitted by an applicable exception or limitation.
No warranties are given. The license may not give you all of the permissions necessary for your intended use. 
For example, other rights such as publicity, privacy, or moral rights may limit how you use the material.


Before proceeding to download any of AeonLabs software solutions for open-source development
 and/or PCB hardware electronics development make sure you are choosing the right license for your project. See 
https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/wiki/AeonLabs-Solutions-for-Open-Hardware-&-Source-Development
 for Open Hardware & Source Development for more information.

*/

GITHUB_API_CLASS::GITHUB_API_CLASS(){}

void GITHUB_API_CLASS::init() {}  

// Function to get the SHA of a file from GitHub
void GITHUB_API_CLASS::getFileSHAFromGitHub(WiFiClientSecure *client, const char* token, const char* repoOwner, const char* repoName, const char* filePath, const char* branch, const char* branch) {

  // Create the URL for the file in the repository
  String url = "/repos/" + String(repoOwner) + "/" + String(repoName) + "/contents/" + String(filePath) + "?ref=" + String(branch);
  // Make sure to trust the GitHub SSL certificate
  client.setInsecure();  // Bypasses SSL certificate verification
  // Connect to GitHub
  if (client.connect("api.github.com", 443)) {
    log_i("Connected to GitHub");

    // Send the GET request
    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: api.github.com");
    client.print("Authorization: token ");
    client.println(token);                 // Send the GitHub PAT for authorization
    client.println("User-Agent: ESP32");
    client.println("Connection: close");
    client.println();

    // Wait for the response
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        break;  // End of headers
      }
    }

    // Read the response body
    String jsonResponse = client.readString();
    log_i("Received JSON response:");

    // Print the JSON response for debugging
    log_i(jsonResponse);

    // Parse the JSON response
    StaticJsonDocument<4000> doc;
    DeserializationError error = deserializeJson(doc, jsonResponse);
    if (error) {
      log_i("Failed to parse JSON: ");
      log_i(error.c_str());
      return;
    }

    // Get the SHA of the file
    const char* sha = doc["sha"];
    if (sha) {
      log_i("File SHA: ");
      log_i(sha);
    } else {
     log_i("SHA not found in the response.");
    }
  } else {
    log_i("Connection to GitHub failed");
  }
}