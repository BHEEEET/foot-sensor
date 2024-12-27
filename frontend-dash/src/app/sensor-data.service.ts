import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root',
})
export class SensorDataService {
  private apiUrl = 'http://127.0.0.1:8080/api/data';  // Original endpoint for all data
  private countApiUrl = 'http://127.0.0.1:8080/api/data/count';  // New endpoint for data count

  constructor(private http: HttpClient) {}

  // This method fetches the full sensor data from the backend API
  getSensorData(): Observable<any> {
    return this.http.get<any>(this.apiUrl);
  }

  // This method fetches the sensor data count from the backend API
  getDataCount(): Observable<any> {
    return this.http.get<any>(this.countApiUrl);
  }
}
