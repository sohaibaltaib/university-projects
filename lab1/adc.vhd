library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity adc is
	port (
		clk      : in  std_logic;
		reset    : in  std_logic;

		config   : in std_logic_vector(15 downto 0);       
		channels : out std_logic_vector(3 downto 0);       
		adc_data : out std_logic_vector(11 downto 0);       

		sdo      : in  std_logic;
		convg    : out  std_logic;
		sck      : out  std_logic;
		sdi      : out  std_logic
		
	);

end entity adc;

architecture adc_arch of adc is

end architecture;
