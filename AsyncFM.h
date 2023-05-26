#ifndef AsyncFM_h
#define AsyncFM_h

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// https://github.com/smford/esp32-asyncwebserver-fileupload-example

const char AsyncFM_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>File Manager</title>
<meta charset="UTF-8">
<link rel="icon" href="data:,">
<script src="AsyncFM.js"></script>
<style>body{font-family:monospace;font-size:1.2em;}</style>
</head>
<body>
<h3>SPIFFS File Manager</h3>
<p>Total: %TOTAL%</p>
<p>Used: %USED%</p>
<p>Free: %FREE%</p>
<button onclick="history.back()">Go Back</button>
<button onclick="getList()">List Files</button>
<button onclick="uploadButton()">Upload File</button>
<button onclick="format()">Format Filesystem</button>
<p id="status"></p>
<h3 id="header"></h3>
<p id="details"></p>
</body>
</html>
)rawliteral";

// https://www.mischianti.org/online-converter-file-to-cpp-gzip-byte-array-3/

// File: AsyncFM.js.gz, Size: 864
#define AsyncFM_js_gz_len 864
const uint8_t AsyncFM_js_gz[] PROGMEM = {
    0x1F, 0x8B, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xB5, 0x55, 0x6D, 0x6F, 0xDB, 0x36,
    0x10, 0xFE, 0x1E, 0x20, 0xFF, 0xE1, 0x46, 0x60, 0x93, 0xBC, 0x79, 0x8A, 0x8B, 0x7E, 0x9B, 0xE3,
    0x14, 0x4E, 0xEB, 0x36, 0x01, 0x9C, 0x36, 0xE8, 0x3C, 0x74, 0xC0, 0x36, 0x04, 0x8C, 0x79, 0x8A,
    0x85, 0x49, 0xA4, 0x4A, 0x9D, 0xEA, 0x18, 0x43, 0xFF, 0xFB, 0x8E, 0x94, 0x65, 0xBD, 0xD8, 0x5D,
    0x5C, 0x6C, 0xD3, 0x07, 0xEB, 0x44, 0xDF, 0xF3, 0xDC, 0xFB, 0x31, 0x2E, 0xF5, 0x92, 0x12, 0xA3,
    0xE1, 0x2E, 0xC4, 0x74, 0x00, 0x7F, 0x9D, 0x9E, 0x00, 0x3F, 0x16, 0xA9, 0xB4, 0x1A, 0x94, 0x59,
    0x96, 0x19, 0x6A, 0x8A, 0x1E, 0x90, 0x66, 0x29, 0x3A, 0xF1, 0x72, 0x73, 0xAD, 0x9C, 0xE6, 0xF8,
    0xF4, 0xE4, 0xF3, 0xE9, 0x49, 0x5C, 0xA3, 0x63, 0x63, 0x33, 0x49, 0xE1, 0x8E, 0x20, 0x89, 0x21,
    0x5C, 0x1A, 0x1D, 0x27, 0x36, 0x0B, 0x83, 0x0F, 0xD3, 0xF7, 0x6F, 0xAF, 0xDF, 0xBE, 0x19, 0xC2,
    0x74, 0x3E, 0x87, 0x57, 0xD3, 0xC5, 0x14, 0x3E, 0x5C, 0xB3, 0x74, 0x39, 0x83, 0xF9, 0xBB, 0x9F,
    0x17, 0xDF, 0xFC, 0xAE, 0xE1, 0xD6, 0x9A, 0x25, 0xA2, 0x82, 0x75, 0x42, 0x2B, 0x78, 0xED, 0xB9,
    0x5E, 0x04, 0x03, 0x98, 0x4C, 0x80, 0x6C, 0x89, 0x3B, 0x56, 0xF7, 0xA4, 0x48, 0x20, 0x61, 0x02,
    0x1A, 0xD7, 0xF0, 0xEB, 0xCD, 0xFC, 0x8A, 0x28, 0x7F, 0x8F, 0x1F, 0x4B, 0x2C, 0xD8, 0xFA, 0xB8,
    0x51, 0x93, 0x91, 0xC9, 0x51, 0x87, 0xE2, 0xCD, 0x6C, 0x21, 0x86, 0x20, 0xCE, 0xE2, 0xEC, 0xAE,
    0xF2, 0x91, 0xBF, 0x62, 0x99, 0x16, 0xD8, 0x55, 0x2E, 0x50, 0xAB, 0x0E, 0xFE, 0x2E, 0x14, 0x05,
    0x49, 0x2A, 0x0B, 0x31, 0x88, 0x12, 0xAD, 0xD1, 0x5E, 0x2D, 0x6E, 0xE6, 0x6C, 0x56, 0x46, 0x16,
    0x8B, 0xDC, 0xE8, 0x02, 0x17, 0xF8, 0x48, 0x5B, 0xC0, 0xE7, 0x6E, 0x32, 0x38, 0x5B, 0xF3, 0xA4,
    0x68, 0x65, 0xE3, 0x08, 0x9F, 0x0F, 0xF8, 0x9B, 0x32, 0x47, 0xDF, 0xDB, 0x9E, 0xA7, 0xEC, 0xE5,
    0x0A, 0xA5, 0x42, 0xDB, 0xF3, 0x52, 0xBC, 0x4E, 0x52, 0x2C, 0x44, 0xA3, 0xA5, 0x90, 0x64, 0x92,
    0x3E, 0x19, 0x4C, 0x3B, 0x0A, 0xE9, 0x5F, 0x61, 0xAC, 0x87, 0x4E, 0xEC, 0xC4, 0x52, 0x3A, 0x13,
    0x3E, 0xA5, 0x6C, 0xE6, 0x85, 0x96, 0x19, 0x4E, 0x04, 0xFC, 0x00, 0xB1, 0xE6, 0x1F, 0xF1, 0x5D,
    0x05, 0xF4, 0x27, 0x2C, 0x8E, 0x9B, 0x86, 0xE0, 0x2F, 0x57, 0x52, 0x76, 0x86, 0x49, 0x50, 0xFC,
    0xFB, 0xB2, 0x96, 0xFF, 0x43, 0x2D, 0xDD, 0xB3, 0x2B, 0x60, 0x5D, 0x5F, 0x40, 0xB6, 0xD2, 0x8D,
    0xC1, 0xAC, 0x75, 0x6A, 0xA4, 0xEA, 0x46, 0xF1, 0x25, 0x4B, 0x42, 0xB4, 0xD8, 0xD7, 0x89, 0x66,
    0x74, 0x15, 0x08, 0x47, 0x20, 0xEE, 0xEE, 0x53, 0xA9, 0xFF, 0x14, 0xFF, 0x60, 0xAC, 0x20, 0x8B,
    0x32, 0xFB, 0x0F, 0x4D, 0x45, 0x31, 0x4F, 0x76, 0xD1, 0xC4, 0xD7, 0xAD, 0x7C, 0x99, 0xBB, 0xC8,
    0x2E, 0x4B, 0x22, 0xAE, 0xFF, 0xCE, 0xE8, 0x93, 0x06, 0xBF, 0xD8, 0x8C, 0xBF, 0x78, 0x3E, 0x70,
    0x3D, 0x29, 0x9E, 0xE8, 0x48, 0x71, 0xEE, 0xA6, 0x14, 0x90, 0x3D, 0xD9, 0xE4, 0x38, 0x09, 0xB2,
    0x32, 0xA5, 0x24, 0x97, 0x96, 0xCE, 0xDC, 0xF9, 0x8F, 0x4A, 0x92, 0x0C, 0x20, 0x43, 0x5A, 0x19,
    0x35, 0x09, 0x72, 0x53, 0x50, 0x70, 0xC1, 0x7D, 0xD6, 0x04, 0x2C, 0xCE, 0x13, 0x9D, 0x97, 0x04,
    0x15, 0xDA, 0xB5, 0x67, 0x00, 0x89, 0xAA, 0xA4, 0x67, 0x01, 0x18, 0xBD, 0x5C, 0x49, 0xFD, 0xC0,
    0x7F, 0x55, 0x31, 0x86, 0x83, 0xE0, 0xE2, 0xFC, 0xDE, 0xF6, 0x39, 0x72, 0x6B, 0x1E, 0xB8, 0x35,
    0x0A, 0x0F, 0xAD, 0x3F, 0x2E, 0xA5, 0x0D, 0xE0, 0x93, 0x4C, 0x4B, 0x46, 0x8F, 0xD8, 0x09, 0xF9,
    0x38, 0x09, 0x9E, 0x8D, 0x58, 0x2A, 0x68, 0x93, 0xF2, 0xD9, 0x3A, 0x51, 0xB4, 0xFA, 0xE9, 0xF9,
    0x68, 0x94, 0x3F, 0x8E, 0x99, 0xF5, 0xAC, 0xC6, 0xED, 0x91, 0x7B, 0xD6, 0xFB, 0x0D, 0x61, 0xF1,
    0xD2, 0x94, 0x9A, 0xBC, 0x6E, 0x5F, 0xC9, 0x87, 0x7B, 0x21, 0xC6, 0x87, 0x0A, 0xD3, 0xDD, 0x2B,
    0x31, 0x67, 0x8D, 0xD3, 0xE9, 0x03, 0x74, 0x85, 0x75, 0x83, 0xFF, 0xDB, 0xE8, 0x8F, 0x71, 0xA3,
    0xA1, 0xB6, 0x63, 0xE5, 0xF6, 0xEA, 0x2B, 0x4E, 0xE0, 0xAE, 0xEC, 0x2A, 0x92, 0x79, 0xEE, 0x86,
    0x65, 0x8B, 0xE6, 0x69, 0x1A, 0x8C, 0xBF, 0x62, 0x63, 0x55, 0xEE, 0x44, 0x52, 0xA9, 0xD9, 0x27,
    0xBE, 0x1A, 0xDC, 0xC8, 0x20, 0x97, 0x32, 0x14, 0x75, 0xE8, 0xCC, 0x58, 0x8B, 0x57, 0x52, 0xAB,
    0x14, 0xED, 0xDE, 0x3A, 0xDB, 0x07, 0xFB, 0xA9, 0x1A, 0x72, 0xA9, 0x5E, 0x9A, 0x2C, 0x77, 0x8B,
    0xE2, 0x08, 0x0C, 0x5A, 0x6B, 0xAC, 0x07, 0xCD, 0x9C, 0x74, 0x04, 0x42, 0xDE, 0x1B, 0x4B, 0x1E,
    0x31, 0x75, 0xD2, 0x1E, 0xA2, 0x5A, 0x33, 0xB7, 0x7C, 0x3F, 0xD5, 0xEB, 0xB8, 0x8A, 0x56, 0xF4,
    0x36, 0xB1, 0xEA, 0xDF, 0x84, 0xBD, 0x80, 0x43, 0x74, 0x76, 0xDB, 0x43, 0xD4, 0x54, 0xFE, 0xF0,
    0x9C, 0xF0, 0x35, 0xE8, 0x56, 0xA7, 0xC7, 0x45, 0xDB, 0x03, 0x5E, 0xAB, 0xE0, 0x71, 0xA2, 0x55,
    0x9F, 0x9C, 0x31, 0x61, 0x47, 0xED, 0x6C, 0x8B, 0x22, 0x43, 0x92, 0x2F, 0xF3, 0xEF, 0x81, 0xFB,
    0xB3, 0x19, 0xCE, 0x56, 0x23, 0xB3, 0x65, 0xDF, 0xC9, 0xCC, 0x70, 0x23, 0x69, 0x15, 0x59, 0x76,
    0x47, 0x85, 0x79, 0xEB, 0x5A, 0x39, 0x38, 0xEA, 0x1D, 0x5D, 0xE7, 0xD4, 0xB7, 0xDB, 0x96, 0x44,
    0x15, 0x45, 0x11, 0x70, 0xB5, 0x24, 0x6F, 0xAE, 0xB5, 0x4C, 0x48, 0xB4, 0xD6, 0x7E, 0x0E, 0x17,
    0x13, 0xE7, 0xC9, 0xE0, 0x88, 0xE5, 0x75, 0xDB, 0x50, 0x0C, 0x61, 0x6D, 0x13, 0x4A, 0xF4, 0x03,
    0xB8, 0xEE, 0x04, 0x32, 0xFE, 0x5D, 0x6C, 0xB8, 0x82, 0x59, 0x7F, 0x30, 0x9A, 0x66, 0xD9, 0xCF,
    0xF7, 0x61, 0x43, 0x6E, 0x19, 0x41, 0x9D, 0x70, 0xF1, 0x54, 0x92, 0xEA, 0x2C, 0xB6, 0x2F, 0x86,
    0xAE, 0x7D, 0xDF, 0x77, 0xC7, 0x1A, 0xAF, 0x17, 0x22, 0x2F, 0xC0, 0xCA, 0x78, 0x97, 0xCB, 0x77,
    0xE4, 0x57, 0x72, 0x79, 0xCC, 0x8E, 0xEC, 0x6F, 0x8F, 0x1E, 0xE9, 0xCD, 0xD6, 0x09, 0x00, 0x00
};

class AsyncFMClass {

public:
    void begin(AsyncWebServer* server, const char* username = "", const char* password = "");

private:
    AsyncWebServer* _server;
    String _username = "";
    String _password = "";
    bool _authRequired = false;
    static String list();
    static String processor(const String& var);
    static String humanReadableSize(const size_t bytes);
    static void handleUpload(AsyncWebServerRequest* request, String filename, size_t index, uint8_t* data, size_t len, bool final);
};

void AsyncFMClass::begin(AsyncWebServer* server, const char* username, const char* password)
{
    _server = server;

    if (strlen(username) > 0) {
        _authRequired = true;
        _username = username;
        _password = password;
    } else {
        _authRequired = false;
        _username = "";
        _password = "";
    }

    _server->on("/filemanager", HTTP_GET, [&](AsyncWebServerRequest* request) {
        if (_authRequired) {
            if (!request->authenticate(_username.c_str(), _password.c_str())) {
                return request->requestAuthentication();
            }
        }
        request->send_P(200, "text/html", AsyncFM_html, processor);
    });

    _server->on("/AsyncFM.js", HTTP_GET, [&](AsyncWebServerRequest* request) {
        AsyncWebServerResponse* response = request->beginResponse_P(200, "text/javascript", AsyncFM_js_gz, AsyncFM_js_gz_len);
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    _server->on(
        "/fm_upload", HTTP_POST, [&](AsyncWebServerRequest* request) { request->send(200); }, handleUpload);

    _server->on("/fm_list", HTTP_GET, [&](AsyncWebServerRequest* request) { request->send(200, "text/plain", list()); });

    _server->on("/fm_format", HTTP_GET, [&](AsyncWebServerRequest* request) {
        if (SPIFFS.format())
            request->send(200, "text/plain", "Format complete");
        else
            request->send(400, "text/plain", "ERROR: formatting failed!");
    });

    _server->on("/fm_file", HTTP_GET, [&](AsyncWebServerRequest* request) {
        if (request->hasParam("name") && request->hasParam("action")) {
            String fileName = request->getParam("name")->value();
            if (!fileName.startsWith("/"))
                fileName = "/" + fileName;
            const char* action = request->getParam("action")->value().c_str();
            if (!SPIFFS.exists(fileName)) {
                request->send(400, "text/plain", "ERROR: file does not exist");
            } else {
                if (strcmp(action, "download") == 0) {
                    request->send(SPIFFS, fileName, "application/octet-stream");
                } else if (strcmp(action, "delete") == 0) {
                    SPIFFS.remove(fileName);
                    request->send(200, "text/plain", "Deleted File: " + String(fileName));
                } else if (strcmp(action, "stream") == 0) {
                    request->send(SPIFFS, fileName, "text/plain");
                } else {
                    request->send(400, "text/plain", "ERROR: invalid action param supplied");
                }
            }
        }
    });
}

String AsyncFMClass::humanReadableSize(const size_t bytes)
{
    if (bytes < 1024)
        return String(bytes) + " B";
    else if (bytes < (1024 * 1024))
        return String(bytes / 1024.0) + " KB";
    else
        return String(bytes / 1024.0 / 1024.0) + " MB";
}

String AsyncFMClass::processor(const String& var)
{
    if (var == "FREE")
        return humanReadableSize((SPIFFS.totalBytes() - SPIFFS.usedBytes()));

    if (var == "USED")
        return humanReadableSize(SPIFFS.usedBytes());

    if (var == "TOTAL")
        return humanReadableSize(SPIFFS.totalBytes());

    return String();
}

void AsyncFMClass::handleUpload(AsyncWebServerRequest* request, String filename, size_t index, uint8_t* data, size_t len, bool final)
{
    if (!index) {
        // open the file on first call and store the file handle in the request object
        request->_tempFile = SPIFFS.open("/" + filename, "w");
    }
    if (len) {
        // stream the incoming chunk to the opened file
        request->_tempFile.write(data, len);
    }
    if (final) {
        // close the file handle as the upload is now done
        request->_tempFile.close();
        request->redirect("/");
    }
}

String AsyncFMClass::list()
{
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    String ret = "<table><tr><th align='left'>Name</th><th align='left'>Size</th><th></th><th></th></tr>";
    while (file) {
        String name = file.name();
        ret += "<tr align='left'><td>";
        ret += name;
        ret += "</td><td>";
        ret += humanReadableSize(file.size());
        ret += "</td>";
        ret += "<td><button onclick=\"action('";
        ret += name;
        ret += "', 'download')\">Download</button>";
        ret += "<td><button onclick=\"action('";
        ret += name;
        ret += "', 'stream')\">Stream</button>";
        ret += "<td><button onclick=\"action('";
        ret += name;
        ret += "', 'delete')\">Delete</button></tr>";
        file = root.openNextFile();
    }
    ret += "</table>";
    file.close();
    root.close();
    return ret;
}

AsyncFMClass AsyncFM;

#endif // AsyncFM_h
