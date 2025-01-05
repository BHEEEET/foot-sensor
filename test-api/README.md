# foot-sensor API
## Setup environment
### 1. Create python env
```
python3 -m venv env

# Activate the environment
.\env\Script\activate
```

### 2. Install the dependencies
```
pip install -r requiremnts.txt
```

### 3. Verify dependencies
```
pip freeze
```

# API Endpoints Overzicht

## Sensor Data Endpoints
1. **POST /api/data**
   - **Doel**: Sla binnenkomende sensorgegevens op.
   - **Input**: JSON-object met sensorgegevens.
   - **Response**: `{ "message": "Data saved successfully" }`

2. **GET /api/data**
   - **Doel**: Haal alle sensorgegevens op.
   - **Response**: `[ { "key1": "value1", "key2": "value2" }, ... ]`

3. **GET /api/data/count**
   - **Doel**: Telling van sensorgegevens per dag.
   - **Response**:
     ```json
     [
       { "_id": "YYYY-MM-DD", "count": X },
       ...
     ]
     ```

4. **GET /api/data/user-count**
   - **Doel**: Dagelijkse telling van sensorgegevens per gebruiker.
   - **Response**:
     ```json
     [
       { "date": "YYYY-MM-DD", "users": [ { "user": "name", "count": X }, ... ] },
       ...
     ]
     ```

---

## Reward Data Endpoints
1. **POST /api/reward**
   - **Doel**: Voeg een nieuwe beloning toe.
   - **Input**: JSON-object met beloningsgegevens.
   - **Response**: `{ "message": "Reward granted successfully" }`

2. **GET /api/reward**
   - **Doel**: Haal alle beloningen op.
   - **Response**: `[ { "reward_field1": "value", "reward_field2": "value" }, ... ]`

3. **GET /api/reward/total**
   - **Doel**: Totale beloningspunten per gebruiker.
   - **Response**:
     ```json
     [
       { "user": "name", "total_points": X },
       ...
     ]
     ```

4. **GET /api/reward/total-per-day**
   - **Doel**: Dagelijkse totale beloningspunten per gebruiker.
   - **Response**:
     ```json
     [
       { "date": "YYYY-MM-DD", "users": [ { "user": "name", "total_points": X }, ... ] },
       ...
     ]
     ```