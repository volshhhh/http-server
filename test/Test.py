import socket
import unittest
import subprocess
import time

class TestHttpServer(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.server_process = subprocess.Popen(["../build/server"])
        time.sleep(1)

    @classmethod
    def tearDownClass(cls):
        cls.server_process.terminate()

    def test_echo_endpoint(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect(("localhost", 4221))
            
            request = (
                "GET /echo/abc HTTP/1.1\r\n"
                "Host: localhost:4221\r\n"
                "Connection: close\r\n"
                "\r\n"
            )
            s.sendall(request.encode())
            
            response = s.recv(4096).decode()
            
            self.assertIn("HTTP/1.1 200 OK", response)
            self.assertIn("abc", response)

    def test_user_agent_endpoint(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect(("localhost", 4221))
            
            request = (
                "GET /user-agent HTTP/1.1\r\n"
                "Host: localhost:4221\r\n"
                "User-Agent: curl/7.81.0\r\n"
                "Connection: close\r\n"
                "\r\n"
            )
            s.sendall(request.encode())
            
            response = s.recv(4096).decode()
            
            self.assertIn("HTTP/1.1 200 OK", response)
            self.assertIn("curl/7.81.0", response)


    def test_not_found_endpoint(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect(("localhost", 4221))
            
            request = (
                "GET /hehe HTTP/1.1\r\n"
                "Host: localhost:4221\r\n"
                "Connection: close\r\n"
                "\r\n"
            )
            s.sendall(request.encode())
            
            response = s.recv(4096).decode()
            
            self.assertIn("HTTP/1.1 404 Not Found", response)
            self.assertIn("", response)
if __name__ == "__main__":
    unittest.main()