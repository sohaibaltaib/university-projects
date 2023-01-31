library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
use ieee.std_logic_unsigned.all;



		
entity adc is
	port (
		clk		: in std_logic;
		reset    : in  std_logic;

		config   : in std_logic_vector(15 downto 0);  
		
		adc_data : out std_logic_vector(11 downto 0);       
	   channels : out std_logic_vector(3 downto 0);       

		sdo      : in  std_logic;
		convg    : out  std_logic;
		sck      : out  std_logic;
		sdi      : out  std_logic
		
	);

end entity adc;

architecture adc_arch of adc is

    signal counter : integer range 0 to 99 := 0; -- convst start pulse correesponds to a 2 microseconds delay (could be 100 pulses) 
    signal count2 : integer range 0 to 70;  -- convst start pulse 80 counts corresponds to 1.6 microseconds using 50 MHz or 1.3 with 65 counts

    signal pulse :   std_logic;
    signal convg_pulse :   std_logic;
    signal current_channel :   STD_LOGIC_VECTOR(3 downto 0);
    signal next_channel :   STD_LOGIC_VECTOR(3 downto 0);
	 
begin
	-- convst clock 
	process (clk, config, current_channel)
	begin
		if rising_edge(clk) then
		counter <= counter +1;
			if (counter < 100) then
				convg <= '0';
			else
				convg <= '1';
				counter <= 0;
			end if;
		end if;
		
		if falling_edge(convg) then
		  next_channel <= (others => '0');
        for i in 0 to 15 loop
            if (config(i) = '1') then
                if (to_integer(unsigned(current_channel)) < i) then
                    next_channel <= std_logic_vector(to_unsigned(i, next_channel'length));
                    exit;
                end if;
            end if;
        end loop;
		end if;
	end process;
	
	
		-- finding the next set 1-bit (next channel) in the config register
--	process (config, current_channel)
--    begin
--		if falling_edge(convg) then 
--        next_channel <= (others => '0');
--        for i in 0 to 15 loop
--            if (config(i) = '1') then
--                if (to_integer(unsigned(current_channel)) < i) then
--                    next_channel <= std_logic_vector(to_unsigned(i, next_channel'length));
--                    exit;
--                end if;
--            end if;
--        end loop;
--   end process;
--	process (clk)
--    begin
--        if rising_edge(clk) then
--			pulse <= not pulse; --toggle the 2 microseconds pulse 
--				
--        end if;
--   end process;
--	 



 
	 
	process (clk)
    begin
        if rising_edge(clk) then
            if (count2 = 70) then
                count2 <= 0;
	  				 convg_pulse <= not convg_pulse;
            else
					 count2 <= count2 + 1; 
            end if;
        end if;
	end process;

	process (channels)
     begin
        case channels is
            when "0000" => adc_data <= "000000000000";
            when "0001" => adc_data <= "000000000001";
            when "0010" => adc_data <= "000000000010";
            when "0011" => adc_data <= "000000000011";
            when "0100" => adc_data <= "000000000100";
            when "0101" => adc_data <= "000000000101";
            when "0110" => adc_data <= "000000000110";
            when "0111" => adc_data <= "000000000111";
            when "1000" => adc_data <= "000000001000";
            when "1001" => adc_data <= "000000001001";
            when "1010" => adc_data <= "000000001010";
            when "1011" => adc_data <= "000000001011";
            when "1100" => adc_data <= "000000001100";
            when "1101" => adc_data <= "000000001101";
            when "1110" => adc_data <= "000000001110";
            when "1111" => adc_data <= "000000001111";
            when others => adc_data <= (others => '0');
        end case;
	end process;

end architecture;