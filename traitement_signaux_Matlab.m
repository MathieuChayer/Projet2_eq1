%% Initialisation
% 
clear all ;   
close all ;  
clc       ; 

%% Chargement des signaux
% 
signal_normal   = loadSignal('SpO2_signal_normal_BPM');
signal_high     = loadSignal('SpO2_signal_high_BPM');
signal_noisy    = loadSignal('SpO2_signal_noisy');
signal_capteur  = 

Fs = 250 ;         % frequence d'echantillonage du signal


%% Création filtres
%
% passe-haut
Fc_haut = 1;             %frequence de coupure du filtre passe haut
Wn_haut=Fc_haut/(Fs/2);  %fréquence de coupure normalisé

[b,a] = butter(6,Wn_haut,'high');

% passe-bas
Fc_bas = 5;              %frequence de coupure du filtre passe bas
Wn_bas = Fc_bas/(Fs/2);  %fréquence de coupure normalisé

[d,c] = butter(6,Wn_bas,'low');

%% Traitement signal normal (signal)
%
signal_red_filtre_haut = filter(b,a,signal_normal.SpO2Red);
filtered_signal_red = filter(d,c,signal_red_filtre_haut);

signal_IR_filtre_haut = filter(b,a,signal_normal.SpO2Infrared);
filtered_signal_IR = filter(d,c,signal_IR_filtre_haut);

% GARDER UN SEUL Xn, cra pareil!
x_red =linspace(0,length(filtered_signal_red)/Fs,length(filtered_signal_red));
x_IR  =linspace(0,length(filtered_signal_IR)/Fs,length(filtered_signal_IR));

%figure(1)
%plot(x_red,signal_normal.SpO2Red,x_red,filtered_signal_red);
%legend('Signal Red','filtered Signal Red');

%figure(2)
%plot(x_IR,signal_normal.SpO2Infrared,x_IR,filtered_signal_IR);
%legend('Signal IR','filtered Signal IR');

%% Traitement signal high (signalH)
%
signalH_red_filtre_haut = filter(b,a,signal_high.SpO2Red);
filtered_signalH_red = filter(d,c,signalH_red_filtre_haut);

signalH_IR_filtre_haut = filter(b,a,signal_high.SpO2Infrared);
filtered_signalH_IR = filter(d,c,signalH_IR_filtre_haut);

xH_red =linspace(0,length(filtered_signalH_red)/Fs,length(filtered_signalH_red));
xH_IR  =linspace(0,length(filtered_signalH_IR)/Fs,length(filtered_signalH_IR));

% figure(3)
%plot(xH_red,signal_high.SpO2Red,xH_red,filtered_signalH_red);
%legend('Signal Red','filtered Signal Red');

figure(4)
plot(xH_IR,signal_high.SpO2Infrared,xH_IR,filtered_signalH_IR);
legend('Signal IR','filtered Signal IR');

%% Traitement signal noisy (signalN)
%
signalN_red_filtre_haut = filter(b,a,signal_noisy.SpO2Red);
filtered_signalN_red = filter(d,c,signalN_red_filtre_haut);

signalN_IR_filtre_haut = filter(b,a,signal_noisy.SpO2Infrared);
filtered_signalN_IR = filter(d,c,signalN_IR_filtre_haut);

xN_red =linspace(0,length(filtered_signalN_red)/Fs,length(filtered_signalN_red));
xN_IR  =linspace(0,length(filtered_signalN_IR)/Fs,length(filtered_signalN_IR));

% figure(5)
% plot(xN_red,signal_noisy.SpO2Red,xN_red,filtered_signalN_red);
% legend('Signal Red','filtered Signal Red');
% 
% figure(6)
% plot(xN_IR,signal_noisy.SpO2Infrared,xN_IR,filtered_signalN_IR);
% legend('Signal IR','filtered Signal IR');


%% Fréquence cardiaque et début saturation AKA CALCULS 
%  (à mettre dans une fonction et envoyé signal) 
%

%Paramètres/variables à changer selon signal
index_start = 1250;
index_last = 6250;
cutoffHorizontal = 125;     % Espacement entre pics (à 250 Hz et un rythme cardiaque max de 110 BPM)
cutoffVertical = 2750;      % Minimum pour être considéré comme un pic 
cutoffVerticalMin = -2000;  % Maximum pour être considéré comme un creux 

%Variables pour tests
max = zeros(40);
a = 1;
min = zeros(40);
b = 1;

signal = signal_normal.SpO2Infrared;
signal_filtre = filtered_signal_IR;
x = x_IR;

compteurMaximums  = 0;
sommeTot = 0;
j = index_start;

 for i = index_start:index_last
     % trouver et compter les max
    if(signal_filtre(i)>cutoffVertical)
        if(signal_filtre(i-1)<signal_filtre(i))
            if(signal_filtre(i)>signal_filtre(i+1))
                compteurMaximums = compteurMaximums + 1;
                max(a)= i;
                a=a+1; 
                i = i + cutoffHorizontal;
            end
        end
    end
    % trouver les min
    if(j<index_last)
        if(signal_filtre(j)<cutoffVerticalMin)
            if(signal_filtre(j-1)>signal_filtre(j))
                if(signal_filtre(j)<signal_filtre(j+1))
                    min(b)= j;
                    b = b + 1; 
                    j = j + cutoffHorizontal;
                end
            end
        end
    end
    j = j + 1 ;
    sommeTot = sommeTot + signal(i);
end



sommeMin = 0;
sommeMax = 0;
indexMin = 0;
indexMax = 0;
for k =1:40
    xline(max(k)/250);
    xline(min(k)/250, '--');
    indexMin = min(k);
    indexMax = max(k);
    if(indexMin > 0)
        sommeMin = sommeMin + signal_filtre(indexMin); 
    end
    if(indexMax>0)
        sommeMax = sommeMax + signal_filtre(indexMax); 
    end
end

bpm = compteurMaximums*15000/(index_last-index_start) % mettre 15000 sous forme de variable
AC  = (sommeMax/(a-1))-(sommeMin/(b-1))
DC  = sommeTot / (index_last-index_start)



%% Fin saturation
% (à mettre dans une deuxième fonction et envoyé AC_1 DC_1, AC_2, DC_2
% R = (AC_1/DC_1)/(AC_2/DC_2)
% SPO2 = (ed1 - R*ed2)/(R*(eo2-ed2)+(ed1-eo1))

% ed: coefficient d'extinction déoxygéné
% eo: coefficient d'extinction oxygéné
% Et
% Où 1: red (660 nm) et 2: IR (880 nm)
% VALEURS DES COEFFICIENTS À TROUVER EN LIGNE (OU APPROX DANS GRAPH DE
% L'ÉNONCÉ)





