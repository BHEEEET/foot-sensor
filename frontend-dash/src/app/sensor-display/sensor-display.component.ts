import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { HttpClientModule } from '@angular/common/http';
import { SensorDataService } from '../sensor-data.service';

@Component({
  selector: 'app-sensor-display',
  standalone: true,
  imports: [CommonModule, HttpClientModule],
  templateUrl: './sensor-display.component.html',
  styleUrl: './sensor-display.component.css'
})
export class SensorDisplayComponent implements OnInit {
  sensorData: any;

  constructor(private sensorDataService: SensorDataService) {}

  ngOnInit(): void {
    this.getSensorData();
  }

  getSensorData(): void {
    this.sensorDataService.getSensorData().subscribe(data => {
      this.sensorData = data;
    });
  }
}
