#include <Windows.h>

//
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg)
	{
		//�E�B���h�D���j�����ꂽ
	case WM_DESTROY:
		//OS�ɑ΂��ăA�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}

	//�W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//�E�B���h�D�T�C�Y
	const int window_width = 1280; //����
	const int window_height = 720; //�c��

	//�E�B���h�D�N���X�̐ݒ�
	WNDCLASSEX w{};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;      //�E�B���h�D�v���V�[�W����ݒ� 
	w.lpszClassName =L"DX12Sample";           //�E�B���h�D�N���X��
	w.hInstance = GetModuleHandle(nullptr);   //�E�B���h�D�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);  //�J�[�\���w��

	//�E�B���h�D�N���X��OS�ɓo�^
	RegisterClassEx(&w);
	//�E�B���h�E�T�C�Y {X���W,Y���W,����,�c��}
	RECT wrc = { 0,0,window_width,window_height };
	//�����ŃT�C�Y��␳
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	//�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello DilectX!!\n");

	return 0;
}