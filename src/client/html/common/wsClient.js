export default class WSClient {
  constructor(url, onMessageCallback) {
    this.url = url;
    this.ws = null;
    this.onMessageCallback = onMessageCallback;
  }

  connect() {
    return new Promise((resolve, reject) => {
      this.ws = new WebSocket(this.url);

      this.ws.onmessage = (event) => {
        this.onMessageCallback(event.data);
      };

      this.ws.onopen = () => {
        console.log('Połączenie WebSocket otwarte');
        resolve();  // Rozwiązujemy Promise, gdy połączenie jest otwarte
      };

      this.ws.onerror = (error) => {
        console.error('Błąd połączenia WebSocket', error);
        reject(error);  // Odrzucamy Promise w przypadku błędu
      };

      this.ws.onclose = () => {
        console.log('Połączenie WebSocket zamknięte');
      };
    });
  }

  send(msg_type_id, payload_msg){
    if (this.ws && this.ws.readyState === this.ws.OPEN) {
      const payload = JSON.stringify({
        msg_type_id: msg_type_id,
        payload: payload_msg
      });
      console.log(payload);
      this.ws.send(payload);
    } else {
      console.error("ws is "+ this.ws + "; readyState:" + this.ws.readyState + " is not open["+this.ws.OPEN+"]");
    }
  }

  send_chat_msg(user_id, message) {
    const payload = {
          user_id: user_id,
          message: message
    };
    this.send(1, payload)
  }

  send_user_register(user_id) {
    const payload = {
          user_id: user_id,
    };
    console.log(payload);
    this.send(2, payload)
  }
}
