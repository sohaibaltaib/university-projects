library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
use ieee.std_logic_unsigned.all;



		
entity adc is
	port (
		clk		: in std_logic;
		reset    : in  std_logic;

		config_reg   : in std_logic_vector(15 downto 0);  
		
		adc_data : out std_logic_vector(11 downto 0) := (others => '0');       
	   channels : out std_logic_vector(3 downto 0);       

		sdo      : in  std_logic;
		convst   : out  std_logic;
		sck      : out  std_logic;
		sdi      : out  std_logic
		
	);

end entity adc;

architecture adc_arch of adc is


-- signal declarations
signal counter: unsigned (19 downto 0);
constant PERIOD: integer := 100;  -- every 2 microseconds
constant PULSE_PERIOD: integer := 1000000; -- for 20 ns
signal pulse : std_logic;
signal next_channel : std_logic_vector(3 downto 0);
signal current_channel : std_logic_vector(3 downto 0);
signal found : std_logic;

-- State machine to control SCK and CONVST signals
signal start_pulse : std_logic;
signal sclk_start : std_logic;
signal sck_counter : unsigned (19 downto 0);
constant SCK_START: integer := 65; -- 1.3 microseconds
constant SCK_END: integer := 80; -- 1.6 microseconds

begin 

	-- Clock and Reset process
	process(clk, reset)
	begin
	
		if (reset = '1') then
			counter <= (others => '0');
			pulse <= '0';
		elsif (rising_edge(clk)) then
			if (counter = PERIOD - 1) then
				counter <= (others => '0');
				pulse <= '1';
			elsif (counter = PULSE_PERIOD - 1) then
				counter <= (others => '0');
				pulse <= '0';
			else
				counter <= counter + 1;
			end if;
		end if;
	end process;

	-- Find the next channel state machine
	process(pulse)
	begin
		if (pulse = '1') then
			current_channel <= next_channel;
			found <= '0';
			for i in 0 to 15 loop
				if (found = '0' and config_reg(i) = '1') then
					if (to_integer(unsigned(current_channel)) < i) then
						next_channel <= std_logic_vector(to_unsigned(i, 4));
						found <= '1';
					end if;
				end if;
			end loop;
			if (found = '0') then
				for i in 0 to 15 loop
					if (config_reg(i) = '1') then
						next_channel <= std_logic_vector(to_unsigned(i, 4));
						found <= '1';
						exit;
					end if;
				end loop;
			end if;
		end if;
	end process;
	
	
	process(current_channel)
	begin
		case current_channel is
			when "0000" => adc_data <= "000000010000";
			when "0001" => adc_data <= "000001001000";
			when "0010" => adc_data <= "000100000100";
			when "0011" => adc_data <= "010000001100";
			when "0100" => adc_data <= "000000010001";
			when "0101" => adc_data <= "000010001010";
			when "0110" => adc_data <= "001000000110";
			when "0111" => adc_data <= "100000001110";
			when "1000" => adc_data <= "000000010001";
			when "1001" => adc_data <= "000001001001";
			when "1010" => adc_data <= "000100000101";
			when "1011" => adc_data <= "010000001101";
			when "1100" => adc_data <= "000000100011";
			when "1101" => adc_data <= "000010001011";
			when "1110" => adc_data <= "001000000111";
			when "1111" => adc_data <= "100000001111";
			when others => adc_data <= (others => '0');
		end case;
	end process;
	

	-- starting the SCLK 
	process(clk, reset, pulse)
	begin
		if (reset = '1') then
			start_pulse <= '0';
			sclk_start <= '0';
			sck_counter <= (others => '0');
		elsif (rising_edge(clk)) then
			if (pulse = '1') then
				start_pulse <= '1';
			end if;
			if (start_pulse = '1') then
				if (sck_counter = SCK_START - 1) then
					sclk_start <= '1';
				elsif (sck_counter = SCK_END - 1) then
					sclk_start <= '0';
					start_pulse <= '0';
					sck_counter <= (others => '0');
				else
					sck_counter <= sck_counter + 1;
				end if;
			end if;
		end if;
	end process;
	
	
		-- Outputs
	channels <= current_channel;
	convst	<= pulse;
--	sdo		<= ;
--	sdi		<= ;
	sck 		<= sclk_start;


end architecture;  