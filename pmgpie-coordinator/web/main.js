"use strict";

const add_thousands_sep = (number) => {
  return number.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ".");
};

const humanizeBigNumber = (number) => {
  if (number < 1000) return number.toString();
  else if (number < 1000000) {
    return (number / 10).toString().substring(0, 4) + " K";
  }
};

const update_stats = (body) => {
  document.getElementById("hex_digits").innerHTML = add_thousands_sep(
    body["hex_digits"]
  );
  document.getElementById("worker_nodes").innerHTML = body["worker_nodes"];
  document.getElementById("throughput").innerHTML = body["throughput"];
  document.getElementById("connected").classList.remove("displaynone");
  document.getElementById("disconnected").classList.add("displaynone");
};

const connect = () => {
  console.log("Main loaded!");

  const ws = new WebSocket("ws://" + window.location.host + "/stats");

  const request_stats = () => {
    if (ws.readyState == ws.OPEN) {
      const msg = { type: "request_stats" };
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

    if (msgParsed["type"] == "publish_stats") update_stats(msgParsed["body"]);
  };
};

connect();
