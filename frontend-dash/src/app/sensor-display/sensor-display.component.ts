import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common'; // To use Angular directives like `ngIf`, `ngFor`
import { SensorDataService } from '../sensor-data.service'; // Import the service

@Component({
  selector: 'app-sensor-display',
  standalone: true,
  imports: [CommonModule], // Import modules directly
  templateUrl: './sensor-display.component.html',
  styleUrls: ['./sensor-display.component.css'],
})
export class SensorDisplayComponent implements OnInit {
  sensorData: any; // This will hold the fetched data
  dataCount: any;

  constructor(private sensorDataService: SensorDataService) {}

  ngOnInit(): void {
    this.getSensorData();
    this.getDataCount();
  }

  /**
   * Fetches sensor data using the SensorDataService.
   */
  getSensorData(): void {
    this.sensorDataService.getSensorData().subscribe(
      (data) => {
        this.sensorData = data; // Assign the fetched data to the component property
      },
      (error) => {
        console.error('Error fetching sensor data:', error);
      }
    );
  }

  getDataCount(): void {
    this.sensorDataService.getDataCount().subscribe(
      (data) => {
        this.dataCount = data; // Assign the fetched data to the component property
      },
      (error) => {
        console.error('Error fetching sensor data:', error);
      }
    );
  }
}
