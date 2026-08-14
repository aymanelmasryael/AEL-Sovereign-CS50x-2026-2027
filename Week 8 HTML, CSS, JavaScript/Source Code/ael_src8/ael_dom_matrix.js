/* AEL Sovereign DOM Matrix v3.0 - Active JavaScript Engine */

let nodeCount = 0;
let asyncTaskCount = 0;
let callStackCount = 0;

// Initialize when DOM fully ready
document.addEventListener('DOMContentLoaded', () => {
    appendTelemetryLog("System Initialization", "AEL Sovereign DOM Matrix v3.0 active.", "log-success");
    startEventLoopTelemetry();
});

// Spawn Dynamic DOM Nodes
function spawnSovereignNode() {
    nodeCount++;
    const grid = document.getElementById('nodes-grid-container');
    
    const nodeEl = document.createElement('div');
    nodeEl.className = 'dom-node-element';
    nodeEl.id = `node-${nodeCount}`;
    
    const timestamp = new Date().toISOString().slice(11, 23);
    nodeEl.innerHTML = `
        <div class="node-title">Node#0x${(nodeCount * 1024 + Math.floor(Math.random()*500)).toString(16).toUpperCase()}</div>
        <div class="node-meta">Runtime allocation timestamp: ${timestamp}</div>
        <div class="node-meta" style="margin-top: 8px; color: #00FF88;">Status: Active Attached</div>
    `;
    
    grid.appendChild(nodeEl);
    document.getElementById('count-active-nodes').innerText = nodeCount;
    appendTelemetryLog("DOM Insertion", `Created dynamic node element ID: ${nodeEl.id}`, "log-normal");
}

// Memory Cleanup / Remove Last Node
function purgeLastNode() {
    const grid = document.getElementById('nodes-grid-container');
    if (grid.lastElementChild) {
        const removedId = grid.lastElementChild.id;
        grid.removeChild(grid.lastElementChild);
        nodeCount = Math.max(0, nodeCount - 1);
        document.getElementById('count-active-nodes').innerText = nodeCount;
        appendTelemetryLog("Memory Purge", `Purged node element ID: ${removedId}`, "log-alert");
    } else {
        appendTelemetryLog("Memory Purge", "DOM grid already empty. No unreferenced nodes found.", "log-alert");
    }
}

// Simulated Asynchronous Fetch API
async function executeVirtualFetch() {
    appendTelemetryLog("Fetch Triggered", "Dispatching virtual non-blocking HTTP GET request...", "log-normal");
    
    // Increment active tasks
    asyncTaskCount++;
    document.getElementById('count-async-tasks').innerText = asyncTaskCount;
    
    // Mock network latency via Promise & setTimeout
    await new Promise(resolve => setTimeout(resolve, 1500));
    
    const mockResponse = {
        meta: {
            secure_handshake: true,
            execution_millis: 148,
            server_node: "ael-sovereign-dubai-node-01"
        },
        payload: [
            { module: "DOM Matrix Visual Engine", state: "OPTIMIZED", version: "3.0" },
            { module: "Sub-Silicon Asynchronous Dispatcher", state: "STABLE", version: "3.0" }
        ]
    };
    
    asyncTaskCount = Math.max(0, asyncTaskCount - 1);
    document.getElementById('count-async-tasks').innerText = asyncTaskCount;
    
    appendTelemetryLog("Fetch Completed", `Received JSON Payload: ${JSON.stringify(mockResponse, null, 2)}`, "log-success");
}

// Event Loop Simulation Telemetry
function startEventLoopTelemetry() {
    setInterval(() => {
        // Simulate Call Stack processing fluctuations
        callStackCount = Math.floor(Math.random() * 3);
        document.getElementById('count-call-stack').innerText = callStackCount;
        
        // Minor background telemetry logging periodically
        if (Math.random() < 0.2) {
            appendTelemetryLog("Event Loop Sync", "Call stack verified cleared. Task Queue checked.", "log-normal");
        }
    }, 2000);
}

// Telemetry Display Logger
function appendTelemetryLog(tag, message, className) {
    const display = document.getElementById('telemetry-console');
    const timeStr = new Date().toISOString().slice(11, 23);
    
    const logItem = document.createElement('div');
    logItem.innerHTML = `<span class="log-time">[${timeStr}] [${tag}]</span> <span class="${className}">${message}</span>`;
    
    display.appendChild(logItem);
    display.scrollTop = display.scrollHeight;
}
