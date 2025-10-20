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

// Example of communication with a device using a text protocol


function powerOn() 
{
    log("Power ON")

    // Sending a power ON command and reading status
    var cmdPow = "OUTP ON\n"
    var cmdStatus = "OUTP?\n"
    var cmd = cmdPow + cmdStatus
    write(cmd)
    wait(200)

    // Reading the answer. Expected response: OUTP ON\n
    var resp = read().toString()
    if (resp !== cmdPow) {
        log("Power ON error")
        return false
    }
    return true
}

function powerOff() 
{
    log("Power OFF")

    // Sending a power OFF command and reading status
    var cmdPow = "OUTP OFF\n"
    var cmdStatus = "OUTP?\n"
    var cmd = cmdPow + cmdStatus
    write(cmd)
    wait(200)

    // Reading the answer. Expected response: OUTP OFF\n
    var resp = read().toString()
    if (resp !== cmdPow) {
        log("Power ON error")
        return false
    }
    return true
}

function getVoltage()
{
    log("Read voltage")

    // Sending GetVoltage command
    var cmd = "MEAS:VOLT?\n"
    write(cmd)
    wait(200)

    // Reading the answer. Expected response: MEAS:VOLT xx\n (xx voltage)
    var resp = read().toString()
    var list = resp.trim().split(' ')
    if (list.length !== 2) {
        log("Voltage reading error")
        return false
    }
    var volt = list[1]
    log("Voltage: " + volt + " V")
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



