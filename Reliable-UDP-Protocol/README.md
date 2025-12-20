# Reliable-UDP-Protocol

**S.H.A.M.** (Sequenced Header for Asynchronous Messaging) is a custom application-layer protocol built on top of UDP. It implements core TCP features—reliability, sequencing, and flow control—to ensure data integrity over an unreliable network.

## 📡 Protocol Features

- **Packet Structure:** Custom 8-byte header containing Sequence Number, Ack Number, Flags (SYN/ACK/FIN), and Window Size.
- **Connection Management:**
  - **Establishment:** 3-Way Handshake (SYN → SYN-ACK → ACK).
  - **Termination:** 4-Way Handshake (FIN → ACK → FIN → ACK).
- **Reliability:**
  - **Retransmission:** Implements Retransmission Timeouts (RTO) for lost packets.
  - **Sequencing:** Handles out-of-order packet arrival.
  - **Cumulative ACKs:** Efficient acknowledgment strategy.
- **Flow Control:** Sliding Window mechanism based on receiver buffer availability.
- **Verification:** MD5 checksum validation upon file transfer completion.

## 🎮 Modes of Operation

1. **File Transfer Mode:** Reliable transfer of text/binary files with simulated packet loss.
2. **Chat Mode:** Real-time bi-directional messaging between client and server.

## ⚙️ Compilation

### Linux
```bash
sudo apt install libssl-dev
make
```

### MacOS
```bash
brew install openssl
make
```

## 🚀 Usage

### Server

Start the server on a specific port.
```bash
./server <port> [--chat] [loss_rate]
# Example: ./server 8080 0.1
```

### Client

**File Transfer:**
```bash
./client <server_ip> <server_port> <input_file> <output_file> [loss_rate]
# Example: ./client 127.0.0.1 8080 data.txt received.txt 0.1
```

**Chat Mode:**
```bash
./client <server_ip> <server_port> --chat [loss_rate]
```

## 📊 Logging

Enable verbose logging by setting `RUDP_LOG=1`. Logs are written to `server_log.txt` and `client_log.txt`.

```bash
export RUDP_LOG=1
```

## 🧪 Testing

The protocol has been tested under various network conditions including:
- Packet loss rates up to 30%
- Out-of-order delivery
- Variable latency conditions
- Large file transfers (>100MB)

## 🔧 Configuration

Key parameters can be adjusted in `sham.h`:
- `MAX_PACKET_SIZE`: Maximum payload per packet (default: 1024 bytes)
- `WINDOW_SIZE`: Initial sender window size (default: 10 packets)
- `TIMEOUT_MS`: Base retransmission timeout (default: 1000ms)
