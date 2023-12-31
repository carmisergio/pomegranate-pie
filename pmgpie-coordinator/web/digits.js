"use strict";

const update_stats = (body) => {
  document.getElementById("digits").innerText = body["digits"].substring(
    0,
    100
  );
  document.getElementById("connected").classList.remove("displaynone");
  document.getElementById("disconnected").classList.add("displaynone");
};

const connect = () => {
  console.log("Main loaded!");

  const ws = new WebSocket("ws://" + window.location.host + "/stats");

  const request_stats = () => {
    if (ws.readyState == ws.OPEN) {
      const msg = { type: "request_digits" };
      ws.send(JSON.stringify(msg));
    }
  };

  let intervalHandle = null;

  ws.onopen = () => {
    console.log("WS connected");

    request_stats();
    // Request stats every second
    intervalHandle = setInterval(request_stats, 1000);
  };

  //   ws.onerror = () => {
  //     setTimeout(connect, 1000);
  //   };

  ws.onclose = () => {
    console.log("WS disconnected");

    document.getElementById("connected").classList.add("displaynone");
    document.getElementById("disconnected").classList.remove("displaynone");

    // Stop requesting stats
    clearInterval(intervalHandle);

    setTimeout(connect, 1000);
  };

  ws.onmessage = (event) => {
    console.log("Message received: " + event.data);

    const msgParsed = JSON.parse(event.data);

    if (msgParsed["type"] == "publish_digits") update_stats(msgParsed["body"]);
  };
};

connect();
