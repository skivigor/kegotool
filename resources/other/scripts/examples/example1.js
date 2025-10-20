/*
* Javascript execution is provided by QJSEngine
* https://doc.qt.io/archives/qt-5.15/qjsengine.html
* 
*  Additional methods:
*   write(data) : send data to port
*   read() : receive data from port
*   wait(ms) : delay in milliseconds
*   log(message) : display message in log window
*/

// Example of communication with a device using a binary protocol

function powerOn() 
{
    log("Power ON")

    // Sending a power ON command
    var cmd = new Uint8Array([0xFF, 0x03, 0x78, 0x21, 0xFF])
    write(cmd.buffer)
    wait(200)

    // Reading the answer. Expected response (ACK): FF 03 78 21 FF
    var resp = new Uint8Array(read())
    if (resp.length !== 5 || resp[1] !== 0x03 || resp[2] !== 0x78 || resp[3] !== 0x21) {
        log("Power ON error")
        return false
    }
    return true
}

function powerOff() 
{
    log("Power OFF")

    // Sending a power OFF command
    var cmd = new Uint8Array([0xFF, 0x03, 0x78, 0x22, 0xFF])
    write(cmd.buffer)
    wait(200)

    // Reading the answer. Expected response (ACK): FF 03 78 22 FF
    var resp = new Uint8Array(read())
    if (resp.length !== 5 || resp[1] !== 0x03 || resp[2] !== 0x78 || resp[3] !== 0x22) {
        log("Power OFF error")
        return false
    }
    return true
}

function getVoltage()
{
    log("Read voltage")

    // Sending GetVoltage command
    var cmd = new Uint8Array([0xFF, 0x03, 0x78, 0x24, 0xFF])
    write(cmd.buffer)
    wait(200)

    // Reading the answer. Expected response: FF 05 78 24 xx xx FF (xx xx - voltage in uint16 format, LSB)
    var resp = new Uint8Array(read())
    if (resp.length !== 7) {
        log("Voltage reading error")
        return false
    }
    var volt = (resp[4] & 0x00FF) | ((resp[5] << 8) & 0xFF00)
    log("Voltage: " + volt + " mV")
    return true
}

function main()
{
    var ret = powerOn()
    if (ret === false) return

    ret = getVoltage()
    if (ret === false) return

    powerOff()
}

main()






