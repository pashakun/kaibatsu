from bs4 import BeautifulSoup
import requests
import cb


print("Initiating a scrape...")
url = "https://www.jma.go.jp/jp/amedas_h/today-44132.html"
page = requests.get(url)
soup = BeautifulSoup(page.content, "html.parser")

print(">>> Data acquired. Extracting values...")
table = soup.find("table", {"id":"tbl_list"})
rows = table.find_all("tr")

results = []
for row in rows[2:]:
    data = row.find_all("td")
    entry = {"time":data[0].get_text(), "temp":data[1].get_text(), "hPa":data[-1].get_text()}
    if entry["temp"] == '\xa0':
        break
    results.append(entry)

values = [value for value in results[-1].values()]
payload = ",".join(values)
print(">>> Payload ready: time = {}, temp= {}, hPa = {}.".format(values[0],values[1],values[2]))

NORDIC_UART_SERVICE_UUID = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
NORDIC_UART_CHARACTERISTIC_UUID = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"


class BlueNRG (object):
    def __init__(self):
        self.peripheral = None
        self.data_characteristics = None


    def did_discover_peripheral(self, p):
        if p.name and 'BlueNRG' in p.name and not self.peripheral:
            self.peripheral = p
            print('>>> Discovered peripheral: %s (%s)' % (p.name, p.uuid))
            print('>>> Connecting to BlueNRG...')
            cb.connect_peripheral(p)


    def did_fail_to_connect_peripheral(self, p, error):
        print('>>> Failed to connect: %s' % (error,))


    def did_disconnect_peripheral(self, p, error):
        print('>>> Disconnected, error: %s' % (error,))
        self.peripheral = None


    def did_connect_peripheral(self, p):
        print('>>> Connected:', p.name)
        print('>>> Discovering services...')
        p.discover_services()


    def did_discover_services(self, p, error):
        for s in p.services:
            if s.uuid == NORDIC_UART_SERVICE_UUID:
                print(">>> Found UART service.")
                p.discover_characteristics(s)


    def did_discover_characteristics(self, s, error):
            print('>>> Did discover characteristics...')
            for c in s.characteristics:
                if c.uuid == NORDIC_UART_CHARACTERISTIC_UUID:
                    self.peripheral.set_notify_value(c, True)
                    self.peripheral.write_characteristic_value(c, payload, False)
                    print(">>> Payload sent.")



mngr = BlueNRG()
cb.set_central_delegate(mngr)
print("Initiating BLE connection...")
print('>>> Scanning for peripherals...')
cb.scan_for_peripherals()


try:
    while True: pass
except KeyboardInterrupt:
    cb.reset()
