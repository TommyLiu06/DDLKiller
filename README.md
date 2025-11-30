# Sync Server for DDLKiller

## Deployment

### 1. Install Requirements

```bash
sudo apt update && sudo apt upgrade

sudo apt install build-essential cmake \  # Build tools
                    libboost-all-dev \  # Boost libraries
                    sqlite3 libsqlite3-dev \  # SQLite3
                    libsqlitecpp-dev \  # SQLiteCpp
                    nlohmann-json3-dev  # nlohmann-json
```

### 2. Build

```bash
mkdir -p build
cd build
cmake ..
make -j$(nproc)
```

NOTICE: If you are building on devices with low memory (<=2GB), consider using `make -j1` to avoid out-of-memory errors.

### 3. Run Server

```bash
./ddl_sync_server <port> <database_path>
```

example:

```bash
./ddl_sync_server 8090 /opt/ddl_sync_server/ddl_sync.db
```

### 4. Config Systemd Service (optional)

Replace `$DDL_SERVER_PORT` and `$DDL_DATABASE_PATH` in the service file `ddl_sync_server.service` accordingly:

```

```

Then run:

```bash
sudo cp ../ddl_sync_server.service /etc/systemd/system/ddl_sync_server.service
sudo systemctl daemon-reload
sudo systemctl enable ddl_sync_server.service
sudo systemctl start ddl_sync_server.service
```

## Local test

### 0. Install `Node.js` and `npm` (if not installed) 

```bash
sudo apt install nodejs npm
```

### 1. Install `wscat`

```bash
npm install wscat
```

### 2. Connect Server

```bash
wscat -c ws://localhost:<port>
```

### 3. Send Test Message

```json
{"type":"modification","content":{"operation":"add","uuid":"20240615093000123","title":"Example Title","description":"This is an example description.","due_date":"2024-12-31"}}
```

and you should see the response messages:

On source client:
```json 
{"content":{"status":"success"},"type":"response"}
```

On other clients:

```json
{"type":"modification","content":{"operation":"add","uuid":"20240615093000123","title":"Example Title","description":"This is an example description.","due_date":"2024-12-31"}}
```
