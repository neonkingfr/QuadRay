/******************************************************************************/
/* Copyright (c) 2013-2017 VectorChief (at github, bitbucket, sourceforge)    */
/* Distributed under the MIT software license, see the accompanying           */
/* file COPYING or http://www.opensource.org/licenses/mit-license.php         */
/******************************************************************************/

#include "RooT.h"

/******************************************************************************/
/****************************   PLATFORM - WIN32   ****************************/
/******************************************************************************/

#include <windows.h>

HINSTANCE   hInst;
HWND        hWnd;
HDC         hWndDC;

HBITMAP     hFrm;
HDC         hFrmDC;
BITMAPINFO  DIBinfo =
{
    sizeof(BITMAPINFOHEADER),           /* biSize */
   +x_row,                              /* biWidth */
   -y_res,                              /* biHeight */
    1,                                  /* biPlanes */
    32,                                 /* biBitCount */
    BI_RGB,                             /* biCompression */
    x_row * y_res * sizeof(rt_ui32),    /* biSizeImage */
    0,                                  /* biXPelsPerMeter */
    0,                                  /* biYPelsPerMeter */
    0,                                  /* biClrUsed */
    0                                   /* biClrImportant */
};

#define _GNU_SOURCE
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/******************************************************************************/
/**********************************   MAIN   **********************************/
/******************************************************************************/

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/*
 * Program's main entry point.
 */
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    /* fill in platform's keymap */
    r_to_p[RK_W]        = KEY_MASK & 'W';
    r_to_p[RK_S]        = KEY_MASK & 'S';
    r_to_p[RK_A]        = KEY_MASK & 'A';
    r_to_p[RK_D]        = KEY_MASK & 'D';

    r_to_p[RK_UP]       = KEY_MASK & VK_UP;
    r_to_p[RK_DOWN]     = KEY_MASK & VK_DOWN;
    r_to_p[RK_LEFT]     = KEY_MASK & VK_LEFT;
    r_to_p[RK_RIGHT]    = KEY_MASK & VK_RIGHT;

    r_to_p[RK_F1]       = KEY_MASK & VK_F1;
    r_to_p[RK_F2]       = KEY_MASK & VK_F2;
    r_to_p[RK_F3]       = KEY_MASK & VK_F3;
    r_to_p[RK_F4]       = KEY_MASK & VK_F4;
    r_to_p[RK_F5]       = KEY_MASK & VK_F5;
    r_to_p[RK_F6]       = KEY_MASK & VK_F6;
    r_to_p[RK_F7]       = KEY_MASK & VK_F7;
    r_to_p[RK_F8]       = KEY_MASK & VK_F8;
    r_to_p[RK_F9]       = KEY_MASK & VK_F9;
    r_to_p[RK_F10]      = KEY_MASK & VK_F10;
    r_to_p[RK_F11]      = KEY_MASK & VK_F11;
    r_to_p[RK_F12]      = KEY_MASK & VK_F12;

    r_to_p[RK_ESCAPE]   = KEY_MASK & VK_ESCAPE;

    /* init internal variables from command-line args */
    args_init(__argc, __argv);

    DIBinfo.bmiHeader.biWidth     = +x_row;
    DIBinfo.bmiHeader.biHeight    = -y_res;
    DIBinfo.bmiHeader.biSizeImage = (x_row * y_res * sizeof(rt_ui32));

    /* create window and register its class */
    MSG msg;
    hInst = hInstance;

    TCHAR wnd_class[] = "RooT";

    WNDCLASSEX wcex;

    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.lpszClassName  = wnd_class;
    wcex.style          = CS_OWNDC;
    wcex.lpfnWndProc    = (WNDPROC)WndProc;
    wcex.hInstance      = hInst;
    wcex.hIcon          = NULL;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = NULL;
    wcex.lpszMenuName   = NULL;
    wcex.hIconSm        = NULL;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;

    if (!RegisterClassEx(&wcex))
    {
        RT_LOGE("Couldn't register class\n");
        return FALSE;
    }


    RECT wRect, cRect;

    hWnd = CreateWindow(wnd_class, title,
                WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
                CW_USEDEFAULT, CW_USEDEFAULT, x_res, y_res,
                NULL, NULL, hInst, NULL);

    if (!hWnd)
    {
        RT_LOGE("Couldn't create window\n");
        return FALSE;
    }

    GetWindowRect(hWnd, &wRect);
    GetClientRect(hWnd, &cRect);
    MoveWindow(hWnd, 100, 50, 2 * x_res - cRect.right,
                              2 * y_res - cRect.bottom, FALSE);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

/*
 * Get system time in milliseconds.
 */
rt_time get_time()
{
    LARGE_INTEGER fr;
    QueryPerformanceFrequency(&fr);
    LARGE_INTEGER tm;
    QueryPerformanceCounter(&tm);
    return (rt_time)(tm.QuadPart * 1000 / fr.QuadPart);
}


#if RT_POINTER == 64
#if RT_ADDRESS == 32

#define RT_ADDRESS_MIN      ((rt_byte *)0x0000000040000000)
#define RT_ADDRESS_MAX      ((rt_byte *)0x0000000080000000)

#else /* RT_ADDRESS == 64 */

#define RT_ADDRESS_MIN      ((rt_byte *)0x0000000140000000)
#define RT_ADDRESS_MAX      ((rt_byte *)0x0000080000000000)

#endif /* RT_ADDRESS */

rt_byte *s_ptr = RT_ADDRESS_MIN;

#endif /* RT_POINTER */


DWORD s_step = 0;

SYSTEM_INFO s_sys = {0};

/*
 * Allocate memory from system heap.
 */
rt_pntr sys_alloc(rt_size size)
{
    pthread_mutex_lock(&mutex);

#if RT_POINTER == 64

    /* loop around RT_ADDRESS_MAX boundary */
    if (s_ptr >= RT_ADDRESS_MAX - size)
    {
        s_ptr  = RT_ADDRESS_MIN;
    }

    if (s_step == 0)
    {
        GetSystemInfo(&s_sys);
        s_step = s_sys.dwAllocationGranularity;
    }

    rt_pntr ptr = VirtualAlloc(s_ptr, size, MEM_COMMIT | MEM_RESERVE,
                  PAGE_READWRITE);

    /* advance with allocation granularity */
    s_ptr = (rt_byte *)ptr + ((size + s_step - 1) / s_step) * s_step;

#else /* RT_POINTER == 32 */

    rt_pntr ptr = malloc(size);

#endif /* RT_POINTER */

#if RT_DEBUG >= 1

    RT_LOGI("ALLOC PTR = %016"PR_Z"X, size = %ld\n", (rt_full)ptr, size);

#endif /* RT_DEBUG */

    pthread_mutex_unlock(&mutex);

#if (RT_POINTER - RT_ADDRESS) != 0

    if ((rt_byte *)ptr >= RT_ADDRESS_MAX - size)
    {
        throw rt_Exception("address exceeded allowed range in sys_alloc");
    }

#endif /* (RT_POINTER - RT_ADDRESS) */

    if (ptr == RT_NULL)
    {
        throw rt_Exception("alloc failed with NULL address in sys_alloc");
    }

    return ptr;
}

/*
 * Free memory from system heap.
 */
rt_void sys_free(rt_pntr ptr, rt_size size)
{
    pthread_mutex_lock(&mutex);

#if RT_POINTER == 64

    VirtualFree(ptr, 0, MEM_RELEASE);

#else /* RT_POINTER == 32 */

    free(ptr);

#endif /* RT_POINTER */

#if RT_DEBUG >= 1

    RT_LOGI("FREED PTR = %016"PR_Z"X, size = %ld\n", (rt_full)ptr, size);

#endif /* RT_DEBUG */

    pthread_mutex_unlock(&mutex);
}

/******************************************************************************/
/*****************************   MULTI-THREADING   ****************************/
/******************************************************************************/

/* platform-specific thread */
struct rt_THREAD
{
    rt_Scene           *scene;
    rt_si32            *cmd;
    rt_si32             index;
    pthread_t           pthr;
    pthread_barrier_t  *barr;
};

/*
 * Worker thread's entry point.
 */
rt_pntr worker_thread(rt_pntr p)
{
    rt_THREAD *thread = (rt_THREAD *)p;

    while (1)
    {
        pthread_barrier_wait(&thread->barr[0]);

        if (thread->scene == RT_NULL)
        {
            break;
        }

        rt_si32 cmd = thread->cmd[0];

        /* if one thread throws an exception,
         * other threads are still allowed to proceed
         * in the same run, but not in the next one */
        if (eout == 0)
        try
        {
            switch (cmd & 0x3)
            {
                case 1:
                thread->scene->update_slice(thread->index, (cmd >> 2) & 0xFF);
                break;

                case 2:
                thread->scene->render_slice(thread->index, (cmd >> 2) & 0xFF);
                break;

                default:
                break;
            };
        }
        catch (rt_Exception e)
        {
            estr[thread->index] = e.err;
            eout = 1;
        }

        pthread_barrier_wait(&thread->barr[1]);
    }

    pthread_barrier_wait(&thread->barr[1]);

    return RT_NULL;
}

/* platform-specific pool
 * of "thnum" threads */
struct rt_THREAD_POOL
{
    rt_Scene           *scene;
    rt_si32             cmd;
    rt_si32             thnum;
    rt_THREAD          *thread;
    pthread_barrier_t   barr[2];
};

/*
 * Initialize platform-specific pool of "thnum" threads.
 */
rt_pntr init_threads(rt_si32 thnum, rt_Scene *scn)
{
    eout = 0; emax = thnum;
    estr = (rt_pstr *)malloc(sizeof(rt_pstr) * thnum);

    if (estr == RT_NULL)
    {
        throw rt_Exception("out of memory for estr in init_threads");
    }

    memset(estr, 0, sizeof(rt_pstr) * thnum);

#if RT_SETAFFINITY

    cpu_set_t cpuset_pr, cpuset_th;
    pthread_t pthr = pthread_self();
    pthread_getaffinity_np(pthr, sizeof(cpu_set_t), &cpuset_pr);

#endif /* RT_SETAFFINITY */

    rt_THREAD_POOL *tpool = (rt_THREAD_POOL *)malloc(sizeof(rt_THREAD_POOL));

    if (tpool == RT_NULL)
    {
        throw rt_Exception("out of memory for tpool in init_threads");
    }

    tpool->scene = scn;
    tpool->cmd = 0;
    tpool->thnum = thnum;
    tpool->thread = (rt_THREAD *)malloc(sizeof(rt_THREAD) * thnum);

    if (tpool->thread == RT_NULL)
    {
        throw rt_Exception("out of memory for thread data in init_threads");
    }

    pthread_barrier_init(&tpool->barr[0], NULL, thnum + 1);
    pthread_barrier_init(&tpool->barr[1], NULL, thnum + 1);

    rt_si32 i, a;

    for (i = 0, a = 0; i < thnum; i++, a++)
    {
        rt_THREAD *thread = tpool->thread;

        thread[i].scene  = scn;
        thread[i].cmd    = &tpool->cmd;
        thread[i].index  = i;
        thread[i].barr   = tpool->barr;

        pthread_create(&thread[i].pthr, NULL, worker_thread, &thread[i]);

#if RT_SETAFFINITY

        while (!CPU_ISSET(a, &cpuset_pr))
        {
            a++;
            if (a == CPU_SETSIZE) a = 0;
        }
        CPU_ZERO(&cpuset_th);
        CPU_SET(a, &cpuset_th);
        pthread_setaffinity_np(thread[i].pthr, sizeof(cpu_set_t), &cpuset_th);

#endif /* RT_SETAFFINITY */
    }

    return tpool;
}

/*
 * Terminate platform-specific pool of "thnum" threads.
 */
rt_void term_threads(rt_pntr tdata, rt_si32 thnum)
{
    rt_si32 i;
    rt_THREAD_POOL *tpool = (rt_THREAD_POOL *)tdata;

    for (i = 0; i < tpool->thnum; i++)
    {
        rt_THREAD *thread = tpool->thread;

        thread[i].scene = RT_NULL;
    }

    pthread_barrier_wait(&tpool->barr[0]);
    pthread_barrier_wait(&tpool->barr[1]);

    for (i = 0; i < tpool->thnum; i++)
    {
        rt_THREAD *thread = tpool->thread;

        pthread_join(thread[i].pthr, NULL);

        thread[i].barr = NULL;
    }

    pthread_barrier_destroy(&tpool->barr[0]);
    pthread_barrier_destroy(&tpool->barr[1]);

    free(tpool->thread);
    free(tpool);

    free(estr);
    estr = RT_NULL;
    eout = emax = 0;
}

/*
 * Task platform-specific pool of "thnum" threads to update scene,
 * block until finished.
 */
rt_void update_scene(rt_pntr tdata, rt_si32 thnum, rt_si32 phase)
{
    rt_THREAD_POOL *tpool = (rt_THREAD_POOL *)tdata;

    tpool->cmd = 1 | ((phase & 0xFF) << 2);
    pthread_barrier_wait(&tpool->barr[0]);
    pthread_barrier_wait(&tpool->barr[1]);
}

/*
 * Task platform-specific pool of "thnum" threads to render scene,
 * block until finished.
 */
rt_void render_scene(rt_pntr tdata, rt_si32 thnum, rt_si32 phase)
{
    rt_THREAD_POOL *tpool = (rt_THREAD_POOL *)tdata;

    tpool->cmd = 2 | ((phase & 0xFF) << 2);
    pthread_barrier_wait(&tpool->barr[0]);
    pthread_barrier_wait(&tpool->barr[1]);
}

/******************************************************************************/
/*******************************   EVENT-LOOP   *******************************/
/******************************************************************************/

/*
 * Set current frame to screen.
 */
rt_void frame_to_screen(rt_ui32 *frame, rt_si32 x_row)
{
    if (frame == RT_NULL)
    {
        return;
    }

    if (frame != ::frame)
    {
        rt_si32 i;

        for (i = 0; i < y_res; i++)
        {
            rt_ui32 *idata = ::frame + i * x_res;

            memcpy(idata, frame + i * x_row, x_res * sizeof(rt_ui32));
        }
    }

    SetDIBitsToDevice(hWndDC, 0, 0, x_res, y_res, 0, 0, 0, y_res,
                                  ::frame, &DIBinfo, DIB_RGB_COLORS);
}

/*
 * Implementation of the event loop.
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    rt_si32 ret, key;

    switch (message)
    {
        case WM_CREATE:
        {
            /* RT_LOGI("Window = %p\n", hWnd); */

            hWndDC = GetDC(hWnd);

            if (hWndDC == NULL)
            {
                return -1;
            }

            hFrm = CreateDIBSection(NULL, &DIBinfo, DIB_RGB_COLORS,
                                            (rt_pntr *)&frame, NULL, 0);

            if (hFrm == NULL || frame == NULL)
            {
                return -1;
            }

            hFrmDC = CreateCompatibleDC(hWndDC);

            if (hFrmDC == NULL)
            {
                return -1;
            }

            SelectObject(hFrmDC, hFrm);

            /* init sys_alloc's mutex */
            pthread_mutex_init(&mutex, NULL);

            ret = main_init();

            if (ret == 0)
            {
                return -1;
            }
        }
        break;

        case WM_KEYDOWN:
        {
            key = wParam;
            /* RT_LOGI("Key press   = %X\n", key); */

            key &= KEY_MASK;
            if (h_keys[key] == 0)
            {
                t_keys[key] = 1;
            }
            h_keys[key] = 1;
        }
        break;

        case WM_KEYUP:
        {
            key = wParam;
            /* RT_LOGI("Key release = %X\n", key); */

            key &= KEY_MASK;
            h_keys[key] = 0;
            r_keys[key] = 1;
        }
        break;

        case WM_MOUSEMOVE:
        break;

        case WM_PAINT:
        {
            ret = main_step();

            if (ret == 0)
            {
                DestroyWindow(hWnd);
            }
        }
        break;

        case WM_DESTROY:
        {
            ret = main_term();

            /* destroy sys_alloc's mutex */
            pthread_mutex_destroy(&mutex);

            if (hFrmDC != NULL)
            {
                DeleteDC(hFrmDC);
                hFrmDC  = NULL;
            }

            if (hFrm   != NULL)
            {
                DeleteObject(hFrm);
                hFrm    = NULL;
            }

            if (hWndDC != NULL)
            {
                ReleaseDC(hWnd, hWndDC);
                hWndDC  = NULL;
            }

            PostQuitMessage(0);
        }
        break;

        default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/