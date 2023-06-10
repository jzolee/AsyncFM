const fm_evt = new EventSource("/fm_evt");
function _(el) { return document.getElementById(el); }
fm_evt.addEventListener("msg", (e) => { _("status").innerHTML = e.data; });
function format() {
    if (confirm('WARNING, ALL DATA WILL BE LOST!\n Proceed with Format?') == true) {
        let a = new XMLHttpRequest();
        a.open("GET", "/fm_format", false);
        a.send();
        _("status").innerHTML = a.responseText;
    }
}
function getList() {
    let a = new XMLHttpRequest();
    a.open("GET", "/fm_list", false);
    a.send();
    _("header").innerHTML = "Files";
    _("details").innerHTML = a.responseText;
}
function action(fn, act) {
    let u = "/fm_file?name=" + fn + "&action=" + act;
    if (act == "delete") {
        let a = new XMLHttpRequest();
        a.open("GET", u, false);
        a.send();
        _("status").innerHTML = a.responseText;
        getList();
    } else if (act == "download") {
        _("status").innerHTML = "";
        window.open(u, "_blank");
    } else if (act == "stream") {
        _("status").innerHTML = "";
        window.open(u, "_blank").focus();
    }
}
function uploadButton() {
    _("status").innerHTML = "";
    _("header").innerHTML = "Upload File"
    _("details").innerHTML = "<form enctype='multipart/form-data' method='post'>" +
        "<input type='file' id='file1' onchange='upload()'><br>" +
        "<progress id='progressBar' value='0' max='100' style='width:300px;'></progress>" +
        "<p id='bytesCount'></p>" +
        "</form>";
}
function upload() {
    let f = _("file1").files[0];
    let d = new FormData();
    d.append("file1", f);
    let a = new XMLHttpRequest();
    a.upload.addEventListener("progress", progressHandler, false);
    a.addEventListener("load", onComplete, false);
    a.addEventListener("error", onError, false);
    a.addEventListener("abort", onAbort, false);
    a.open("POST", "/fm_upload");
    a.send(d);
}
function progressHandler(event) {
    _("bytesCount").innerHTML = "Uploaded " + event.loaded + " bytes";
    let p = (event.loaded / event.total) * 100;
    _("progressBar").value = Math.round(p);
    _("status").innerHTML = Math.round(p) + "% uploaded... please wait";
    if (p >= 100) _("status").innerHTML = "Please wait, writing file to filesystem";
}
function onComplete(event) {
    _("status").innerHTML = "File Uploaded";
    _("progressBar").value = 0;
    getList();
}
function onError(event) {
    _("status").innerHTML = "Upload Failed";
}
function onAbort(event) {
    _("status").innerHTML = "Upload Aborted";
}
