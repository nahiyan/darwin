<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Models\Extension;

class AppController extends Controller
{
    public function index()
    {
        return view('index', ['extensions' => Extension::all()]);
    }
}
