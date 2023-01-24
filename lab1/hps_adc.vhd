library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity hps_adc is
	port(
			clk						: in  std_logic;
			reset						: in  std_logic;
			avs_s1_read	 	 		: in  std_logic;                     -- Avalon read control signal
			avs_s1_write 	 		: in  std_logic;                     -- Avalon write control signal
			avs_s1_address 		: in  std_logic_vector(2 downto 0);  -- Avalon address;  Note: width determines the number of registers created by Platform Designer
			avs_s1_readdata		: out std_logic_vector(31 downto 0); -- Avalon read data bus 
			avs_s1_writedata 		: in  std_logic_vector(31 downto 0); -- Avalon write data bus 
			sdo      : in  std_logic;
			convg    : out  std_logic;
			sck      : out  std_logic;
			sdi      : out  std_logic
	);
end entity;

architecture hps_adc_arch of hps_adc is



end architecture;
