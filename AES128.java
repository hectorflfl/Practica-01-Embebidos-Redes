package com.example.aes128;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

public class AES128 {
	

	 public static byte[] ecbEncrypt(byte sSrc[], String sKey) throws Exception {
	        if (sKey == null) {
	            System.out.print("Key is null");
	            return null;
	        }

	        
	        if (sKey.length() % 16 != 0) {
	            System.out.print("Key not valid");
	            return null;
	        }

	        byte[] raw = sKey.getBytes("utf-8");
	        SecretKeySpec skeySpec = new SecretKeySpec(raw, "AES");
	        Cipher cipher = Cipher.getInstance("AES/ECB/NoPadding");
	        cipher.init(Cipher.ENCRYPT_MODE, skeySpec);
	        byte[] encrypted = cipher.doFinal(sSrc);

	        return encrypted;
	    }

	    
	    public static byte[] ecbDecrypt(byte sSrc[], String sKey) throws Exception {
	        try {
	      
	            if (sKey == null) {
	                System.out.print("Key is null");
	                return null;
	            }

	            
	            if (sKey.length()%16 != 0) {
	                System.out.print("Key not valid");
	                return null;
	            }

	            byte[] raw = sKey.getBytes("utf-8");
	            SecretKeySpec skeySpec = new SecretKeySpec(raw, "AES");
	            Cipher cipher = Cipher.getInstance("AES/ECB/NoPadding");
	            cipher.init(Cipher.DECRYPT_MODE, skeySpec);
	            try {
	                return cipher.doFinal(sSrc);
	            } catch (Exception e) {
	                System.out.println(e.toString());
	                return null;
	            }
	        } catch (Exception ex) {
	            System.out.println(ex.toString());
	            return null;
	        }
	    }
	    

}














