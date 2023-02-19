library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity adc is
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
end entity;	


architecture adc_arc of adc is

	-- signal declarations
	type state_type is (s0, s1, s2);
	signal current_state, next_state : state_type;
	signal config_sdi : std_logic_vector(5 downto 0) := "000010";
	signal cnt, current_channel: integer := 0;
	signal sck_sig, clk_sig, select_ch, sample_ch, data_out : std_logic := '0';
	signal shift_reg : std_logic_vector(11 downto 0) := "000000000000";

	-- Function to look for the next set 1-bit in config_reg
	impure function next_channel return integer is
		begin	
			for i in 0 to 15 loop
				if (i >= current_channel + 1 and i < 16) then
					if(config_reg(i) = '1') then
					 return i;
					 end if;
				end if;
			end loop;
			
			for i in 0 to 15 loop
				if (i <= current_channel - 1) then
					if(config_reg(i) = '1') then
					 return i;
					 end if;
				end if;
			end loop;	
		
		return current_channel;
	end function;	
	
begin

	sck <= sck_sig;
	
-- starting sck clock
	sck_clk: process(clk)
		begin
			if (rising_edge(clk) and cnt > 66 and cnt < 91) then
			sck_sig <= not sck_sig;
			end if;			
	end process;
	
-- starting the bit data clock
	clk_delay: process(clk)
		begin
			if (rising_edge(clk)) then
			clk_sig <= not clk_sig;
			end if;		 	
	end process;	
	
-- cnt to set up the next state logic
	counter: process(clk, reset)
		begin
			if (reset = '1') then
				cnt <= 0;
			elsif (rising_edge(clk)) then
				convst <= '0';
				cnt <= cnt + 1;	
				if (cnt = 99) then
					convst <= '1';
					cnt <= 0;
				elsif (cnt = 1) then
					select_ch <= '1'; 
					sample_ch <= '0'; 
					data_out <= '0';
				elsif (cnt = 63) then
					select_ch <= '0'; 
					sample_ch <= '1'; 
					data_out <= '0';
				elsif (cnt = 91) then
					select_ch <= '0'; 
					sample_ch <= '0'; 
					data_out <= '1';
				end if;
			end if;			
	end process;
	
-- State memory logic
	state_memory: process(clk, reset)
		begin
			if (reset = '1') then
				current_state <= s0;
			elsif (rising_edge(clk)) then
				current_state <= next_state;
			end if;
	end process;
	
	
	next_state_logic: process(clk)
		begin
			if (rising_edge(clk)) then
				case (current_state) is
								  
					when s0 => 
						if (select_ch = '1') then 
							next_state <= s0;
						else 
							next_state <= s1;
						end if;
								  
					when s1 => 
						if (sample_ch = '1') then
							next_state <= s1;
						else
							next_state <= s2;
						end if;
						
					when s2 =>
						if (data_out = '1') then
							next_state <= s2;
						else
							next_state <= s0;
						end if;
					when others => next_state <= s0;
				end case;
			end if;
	end process;
	
	
	output_logic: process(clk)
		variable ch_data_out : std_logic_vector(3 downto 0);
		begin
			if (rising_edge(clk)) then
				case (current_state) is
					when s0 =>
						if (cnt = 5) then
							ch_data_out := std_logic_vector(to_unsigned(current_channel, 4)); 
							current_channel <= next_channel; 
						
						elsif (cnt = 10) then
							case (current_channel) is
								when 0 	=> config_sdi <= "100010";
								when 1	=> config_sdi <= "110010";
								when 2	=> config_sdi <= "100110";
								when 3	=> config_sdi <= "110110"; 
								when 4	=> config_sdi <= "101010"; 
								when 5	=> config_sdi <= "111010"; 
								when 6	=> config_sdi <= "101110"; 
								when 7	=> config_sdi <= "111110"; 
								when 8	=> config_sdi <= "000010"; 
								when 9	=> config_sdi <= "010010"; 
								when 10	=> config_sdi <= "000110"; 
								when 11	=> config_sdi <= "010110"; 
								when 12	=> config_sdi <= "001010"; 
								when 13	=> config_sdi <= "011010";
								when 14	=> config_sdi <= "001110"; 
								when 15	=> config_sdi <= "011110";
								when others => config_sdi <= "100010";
							end case;
						end if;
						
					when s1 =>
						if(clk_sig = '0') then
							sdi <= config_sdi(5);
							config_sdi(5 downto 1) <= config_sdi(4 downto 0);		
							shift_reg(0) <= sdo;
							shift_reg(11 downto 1) <= shift_reg(10 downto 0);
						end if;
						
					when s2 =>
						if (cnt = 95) then
							channel <= ch_data_out;
							adc_data <= shift_reg;
						end if;
			end case;
		end if;
	end process;
	
	
end architecture;
