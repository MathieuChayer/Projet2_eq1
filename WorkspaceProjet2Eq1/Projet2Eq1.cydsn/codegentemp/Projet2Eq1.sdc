# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\Mathieu\OneDrive\Documents\Repos_Projet2\WorkspaceProjet2Eq1\Projet2Eq1.cydsn\Projet2Eq1.cyprj
# Date: Tue, 06 Apr 2021 02:41:14 GMT
#set_units -time ns
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyClk_LF} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyFLL} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/fll}]]
create_clock -name {CyClk_HF0} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/hfclk_0}]]
create_clock -name {CyClk_Fast} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/fastclk}]]
create_clock -name {CyClk_Peri} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/periclk}]]
create_generated_clock -name {CyClk_Slow} -source [get_pins {ClockBlock/periclk}] -edges {1 3 5} [list [get_pins {ClockBlock/slowclk}]]
create_generated_clock -name {MAX_I2C_SCBCLK} -source [get_pins {ClockBlock/periclk}] -edges {1 65 131} [list [get_pins {ClockBlock/ff_div_2}]]
create_generated_clock -name {BMI_I2C_SCBCLK} -source [get_pins {ClockBlock/periclk}] -edges {1 13 27} [list [get_pins {ClockBlock/ff_div_3}]]
create_generated_clock -name {EINK_Clock} -source [get_pins {ClockBlock/periclk}] -edges {1 50001 100001} [list [get_pins {ClockBlock/ff_div_11}]]
create_generated_clock -name {UART_SCBCLK} -source [get_pins {ClockBlock/periclk}] -edges {1 73 145} [list [get_pins {ClockBlock/ff_div_5}]]
create_generated_clock -name {CY_EINK_SPIM_SCBCLK} -source [get_pins {ClockBlock/periclk}] -edges {1 2 3} [list [get_pins {ClockBlock/ff_div_6}]]
create_clock -name {CyPeriClk_App} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/periclk_App}]]
create_clock -name {CyIMO} -period 125 -waveform {0 62.5} [list [get_pins {ClockBlock/imo}]]


# Component constraints for C:\Users\Mathieu\OneDrive\Documents\Repos_Projet2\WorkspaceProjet2Eq1\Projet2Eq1.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\Mathieu\OneDrive\Documents\Repos_Projet2\WorkspaceProjet2Eq1\Projet2Eq1.cydsn\Projet2Eq1.cyprj
# Date: Tue, 06 Apr 2021 02:40:29 GMT
