<?php
// automatically generated by the FlatBuffers compiler, do not modify

namespace Jumper;

use \Google\FlatBuffers\Struct;
use \Google\FlatBuffers\Table;
use \Google\FlatBuffers\ByteBuffer;
use \Google\FlatBuffers\FlatBufferBuilder;

class Member extends Table
{
    /**
     * @param ByteBuffer $bb
     * @return Member
     */
    public static function getRootAsMember(ByteBuffer $bb)
    {
        $obj = new Member();
        return ($obj->init($bb->getInt($bb->getPosition()) + $bb->getPosition(), $bb));
    }

    /**
     * @param int $_i offset
     * @param ByteBuffer $_bb
     * @return Member
     **/
    public function init($_i, ByteBuffer $_bb)
    {
        $this->bb_pos = $_i;
        $this->bb = $_bb;
        return $this;
    }

    /**
     * @param int offset
     * @return double
     */
    public function getChromosomes($j)
    {
        $o = $this->__offset(4);
        return $o != 0 ? $this->bb->getDouble($this->__vector($o) + $j * 8) : 0;
    }

    /**
     * @return int
     */
    public function getChromosomesLength()
    {
        $o = $this->__offset(4);
        return $o != 0 ? $this->__vector_len($o) : 0;
    }

    /**
     * @return float
     */
    public function getScore()
    {
        $o = $this->__offset(6);
        return $o != 0 ? $this->bb->getFloat($o + $this->bb_pos) : 0.0;
    }

    /**
     * @param FlatBufferBuilder $builder
     * @return void
     */
    public static function startMember(FlatBufferBuilder $builder)
    {
        $builder->StartObject(2);
    }

    /**
     * @param FlatBufferBuilder $builder
     * @return Member
     */
    public static function createMember(FlatBufferBuilder $builder, $chromosomes, $score)
    {
        $builder->startObject(2);
        self::addChromosomes($builder, $chromosomes);
        self::addScore($builder, $score);
        $o = $builder->endObject();
        return $o;
    }

    /**
     * @param FlatBufferBuilder $builder
     * @param VectorOffset
     * @return void
     */
    public static function addChromosomes(FlatBufferBuilder $builder, $chromosomes)
    {
        $builder->addOffsetX(0, $chromosomes, 0);
    }

    /**
     * @param FlatBufferBuilder $builder
     * @param array offset array
     * @return int vector offset
     */
    public static function createChromosomesVector(FlatBufferBuilder $builder, array $data)
    {
        $builder->startVector(8, count($data), 8);
        for ($i = count($data) - 1; $i >= 0; $i--) {
            $builder->putDouble($data[$i]);
        }
        return $builder->endVector();
    }

    /**
     * @param FlatBufferBuilder $builder
     * @param int $numElems
     * @return void
     */
    public static function startChromosomesVector(FlatBufferBuilder $builder, $numElems)
    {
        $builder->startVector(8, $numElems, 8);
    }

    /**
     * @param FlatBufferBuilder $builder
     * @param float
     * @return void
     */
    public static function addScore(FlatBufferBuilder $builder, $score)
    {
        $builder->addFloatX(1, $score, 0.0);
    }

    /**
     * @param FlatBufferBuilder $builder
     * @return int table offset
     */
    public static function endMember(FlatBufferBuilder $builder)
    {
        $o = $builder->endObject();
        return $o;
    }
}
