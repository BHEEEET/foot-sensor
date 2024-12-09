import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root',
})
export class SensorDataService {
  private apiUrl = 'http://127.0.0.1:8080/api/data'; 

  constructor(private http: HttpClient) {}

  getSensorData(): Observable<any> {
    return this.http.get<any>(this.apiUrl);
  }
}

