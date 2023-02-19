library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity hps_adc is
    port(
        clk              	: in  std_logic;                     -- system clock
        reset            	: in  std_logic;                     -- system reset (assume active high, change at top level if needed)
        avs_s1_read	 	 	: in  std_logic;                     -- Avalon read control signal
        avs_s1_write 	 	: in  std_logic;                     -- Avalon write control signal
        avs_s1_address 	 	: in  std_logic_vector(4 downto 0);  -- Avalon address;  Note: width determines the number of registers created by Platform Designer
        avs_s1_writedata 	: in  std_logic_vector(31 downto 0); -- Avalon write data bus 
		  avs_s1_readdata	 	: out std_logic_vector(31 downto 0); -- Avalon read data bus 
		  sdo						: in	std_logic;
		  sck						: out std_logic := '0';
		  sdi						: out std_logic := '0'; 
		  convst					: out	std_logic := '0'
    );
end entity hps_adc;

architecture hps_adc_arch of hps_adc is

	-- Signal Declarations
	signal adc_data  	:  std_logic_vector(11 downto 0);
	signal ch_config_0  		:  std_logic_vector(11 downto 0);
	signal ch_config_1  		:  std_logic_vector(11 downto 0);
	signal ch_config_2  		:  std_logic_vector(11 downto 0);
	signal ch_config_3  		:  std_logic_vector(11 downto 0);
	signal ch_config_4  		:  std_logic_vector(11 downto 0);
	signal ch_config_5  		:  std_logic_vector(11 downto 0);
	signal ch_config_6  		:  std_logic_vector(11 downto 0);
	signal ch_config_7  		:  std_logic_vector(11 downto 0);
	signal ch_config_8  		:  std_logic_vector(11 downto 0);
	signal ch_config_9  		:  std_logic_vector(11 downto 0);
	signal ch_config_10 		:  std_logic_vector(11 downto 0);
	signal ch_config_11  		:  std_logic_vector(11 downto 0);
	signal ch_config_12  		:  std_logic_vector(11 downto 0);
	signal ch_config_13  		:  std_logic_vector(11 downto 0);
	signal ch_config_14  		:  std_logic_vector(11 downto 0);
	signal ch_config_15  		:  std_logic_vector(11 downto 0);
	signal config_reg 	:  std_logic_vector(15 downto 0); 
	signal channel 	:  std_logic_vector(3 downto 0);
	
	-- Component Declarations
	
	component adc
		port(
		clk					:	in		std_logic;
		reset					: 	in 	std_logic;
		config_reg			:	in 	std_logic_vector(15 downto 0);
		sdo					: 	in		std_logic;
		channel				:  out	std_logic_vector(3 downto 0);
		adc_data 			:  out	std_logic_vector(11 downto 0);
		sck					: 	out 	std_logic;
		sdi					: 	out 	std_logic; 
		convst				: 	out	std_logic
	);
	end component;

begin
	adc_0 : adc port map(clk => clk, reset => reset, config_reg => config_reg, sdo => sdo, channel => channel, adc_data => adc_data,
								sck => sck, sdi => sdi, convst => convst);
								
	-- Concurrent Statements and processes (including Avalon bus interfacing and register creation)
	process(clk)
		begin
		if(rising_edge(clk)) then
			case (channel) is
				when "0000" => ch_config_0 <= adc_data;
				when "0001" => ch_config_1 <= adc_data;
				when "0010" => ch_config_2 <= adc_data;
				when "0011" => ch_config_3 <= adc_data;
				when "0100" => ch_config_4 <= adc_data;
				when "0101" => ch_config_5 <= adc_data;
				when "0110" => ch_config_6 <= adc_data;
				when "0111" => ch_config_7 <= adc_data;
				when "1000" => ch_config_8 <= adc_data;
				when "1001" => ch_config_9 <= adc_data;
				when "1010" => ch_config_10 <= adc_data;
				when "1011" => ch_config_11 <= adc_data;
				when "1100" => ch_config_12 <= adc_data;
				when "1101" => ch_config_13 <= adc_data;
				when "1110" => ch_config_14 <= adc_data;
				when "1111" => ch_config_15 <= adc_data;
				when others => null;
			end case;	
		end if;
	end process;
		
	
	avalon_register_read : process (clk)
		begin
			if (rising_edge (clk) and avs_s1_read = '1') then
				case avs_s1_address is 
					when "00000" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_0;
					when "00001" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_1;
					when "00010" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_2;
					when "00011" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_3;
					when "00100" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_4;	
					when "00101" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_5;	
					when "00110" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_6;	
					when "00111" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_7;
					when "01000" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_8;	
					when "01001" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_9;	
					when "01010" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_10;
					when "01011" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_11; 
					when "01100" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_12; 
					when "01101" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_13; 
					when "01110" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_14;
					when "01111" => avs_s1_readdata <= (31 downto 12 => '0') & ch_config_15;
					when "10000" => avs_s1_readdata <= (31 downto 16 => '0') & config_reg;	
					when others => avs_s1_readdata <= ( others =>'0'); -- return zeros for unused registers
				end case;
			end if;
	end process ;
	
	avalon_register_write : process (clk , reset)
		begin
			if reset = '1' then
				config_reg <= "0000000000000001";
				
			elsif (rising_edge (clk) and avs_s1_write = '1') then
				case avs_s1_address is
					when "10000" => config_reg <= avs_s1_writedata(15 downto 0);
					when others => null; -- ignore writes to unused registers
				end case;
			end if;
	end process;
	
end architecture;
