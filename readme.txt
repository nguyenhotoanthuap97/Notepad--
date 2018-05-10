Họ tên: Nguyễn Hồ Toàn Thư
MSSV: 1512559
Môi trường lập trình: Visual Studio 2017 trên hệ điều hành Windows 8.1
Chức năng:
	- Luồng sự kiện chính:  +Tạo được giao diện có edit control để nhập liệu
				+Lưu file dạng text có sử dụng SaveFileDialog
				+Open file dạng text có sử dụng OpenFileDialog
				+Hỗ trợ 3 thao tác Cut-Copy-Paste
				+Hiện tại vẫn chỉ đọc và ghi file ANSI, chưa hỗ trợ Unicode.


	-Luồng sự kiện phụ:	+Khi thoát chương trình sẽ bắt sự kiện chưa lưu nội dung đang chỉnh sửa của:
					. Một file đang mở
					. Nội dung chưa được lưu thành file
				+Khi mở file khác sẽ bắt sự kiện chưa lưu lưu nội dung đang chỉnh sửa của:
					. Một file đang mở
					. Nội dung chưa được lưu thành file
				+Có thêm chức năng mở file mới (New)
				+Mỗi khi bắt được sự kiện có ảnh hưởng tới nội dung đang làm việc (Open, New, Exit), xét nội dung đang chỉnh sửa có rỗng (đối với file chưa lưu thành thực thể) hay nội dung có thay đổi so với phiên bản đã được lưu trước đó để bỏ qua, không thông báo người dùng.
	
Link bitbucket repository: https://bitbucket.org/nguyenhotoanthu/windev
