clc;
clear all;
close all;


data = readtable('lab2.csv');

voltage = data{:,1};
resistance = data{:,2};
temperature = data{:,3};


figure();
subplot(1,2,1);
plot(voltage,temperature);
title('�������������� Vo - T (C)');
xlabel('Vo');
ylabel('Temperature (C)');
grid on;

min(resistance)

subplot(1,2,2);
plot(voltage,resistance - 101035.6);
title('�������������� Vo - �Rt - R��� = 101035.6 �');
xlabel('Vo');
ylabel('�Rt');
grid on;




