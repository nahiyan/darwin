<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Session extends Model
{
    use HasFactory;

    public function generations()
    {
        return $this->hasMany('App\Models\Generation');
    }

    public function extension()
    {
        return $this->belongsTo('App\Models\Extension');
    }
}
