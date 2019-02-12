package com.example.aes128;

import java.util.zip.CRC32;
import java.util.zip.Checksum;

public class crc32 {
	
	public static long getCrc32(String input) {
	      
	      // get bytes from string
	      byte bytes[] = input.getBytes();
	        
	      Checksum checksum = new CRC32();
	       
	      // update the current checksum with the specified array of bytes
	      checksum.update(bytes, 0, bytes.length);
		
		return checksum.getValue();
	}

     
	
}
