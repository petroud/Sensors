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
title('Χαρακτηριστική Vo - T (C)');
xlabel('Vo');
ylabel('Temperature (C)');
grid on;

min(resistance)

subplot(1,2,2);
plot(voltage,resistance - 101035.6);
title('Χαρακτηριστική Vo - ΔRt - Rαρχ = 101035.6 Ω');
xlabel('Vo');
ylabel('ΔRt');
grid on;




